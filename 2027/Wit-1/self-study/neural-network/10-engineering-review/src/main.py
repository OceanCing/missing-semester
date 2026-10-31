from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / '_shared'))

import cv2
import core as c


def run(args):
    folder=Path(args.models).expanduser().resolve() if args.models else c.MODELS
    meta=c.read_json(folder/'metadata.json')
    classes=meta['classes']
    if len(classes)!=len(c.CLASSES):
        raise ValueError('Model metadata must contain the same 11 output classes')
    model=c.load_model(folder/'baseline.pt')
    image=cv2.imread(str(folder/'fixtures/digit-07.png'))
    records=[]
    for mode in ('normal','raw','invert'):
        index,score,_=c.infer(model,image,False,mode)
        records.append(dict(case=mode,class_id=index,label=classes[index],score=score))
    index,score,_=c.infer(model,image)
    records.append(dict(case='wrong-label-map',class_id=index,label=list(reversed(classes))[index],score=score))
    import hashlib
    hash_ok=meta['sha256']==hashlib.sha256((folder/'baseline.onnx').read_bytes()).hexdigest()
    return dict(model_hash_matches=hash_ok,model_package=str(folder),cases=records),c.montage([image]*4,[r['case']+':'+r['label'] for r in records],4)

if __name__ == '__main__':
    c.run_experiment(Path(__file__).resolve().parents[1].name, run)
