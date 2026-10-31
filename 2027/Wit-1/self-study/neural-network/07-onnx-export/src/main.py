from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / '_shared'))

import cv2
import numpy as np
import core as c
import hashlib
import shutil


def run(args):
    model=c.load_model(args.checkpoint or None)
    deployment=args.output/'deployment'
    deployment.mkdir(parents=True,exist_ok=True)
    output=deployment/'baseline.onnx'
    operators=c.export_model(model,output)
    c.torch.save(model.state_dict(),deployment/'baseline.pt')
    shutil.copytree(c.MODELS/'fixtures',deployment/'fixtures',dirs_exist_ok=True)
    meta=c.read_json(c.MODELS/'metadata.json')
    meta.update(sha256=hashlib.sha256(output.read_bytes()).hexdigest(),operators=operators,
                torch=c.torch.__version__,checkpoint_source=args.checkpoint or 'bundled baseline')
    c.save_json(deployment/'metadata.json',meta)
    net=cv2.dnn.readNetFromONNX(str(output))
    differences=[]; fixtures=[]
    for item in c.read_json(c.MODELS/'parity.json')['cases']:
        image=cv2.imread(str(c.MODELS/item['image']))
        expected=c.infer(model,image,item['roi'])[2]
        net.setInput(c.preprocess(image,item['roi']))
        difference=float(np.max(np.abs(net.forward().ravel()-expected)))
        differences.append(difference)
        fixtures.append(dict(image=item['image'],roi=int(item['roi']),logits=expected.tolist()))
    c.save_json(deployment/'parity.json',dict(absolute_tolerance=.001,cases=fixtures))
    result=dict(operators=operators,opset=11,cases=len(differences),maximum_logit_difference=max(differences),
                inference_backend='Python OpenCV '+cv2.__version__,tolerance=.001,deployment=str(deployment))
    canvas=np.full((240,900,3),25,np.uint8)
    for i,text in enumerate(('ONNX: static input 1x1x32x32; raw logits', 'Operators: '+','.join(operators),
                             'Cases=%d max logit difference=%.8f'%(len(differences),max(differences)))):
        cv2.putText(canvas,text,(15,45+i*60),cv2.FONT_HERSHEY_SIMPLEX,.55,(230,230,230),1)
    return result,canvas

if __name__ == '__main__':
    c.run_experiment(Path(__file__).resolve().parents[1].name, run)
