from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / '_shared'))

import cv2
import core as c


def run(args):
    model = c.load_model()
    roi = cv2.imread(str(c.MODELS/'fixtures/roi-7.png'))
    modes=['normal','raw','invert']
    result, images, names = {}, [], []
    for mode in modes:
        tensor=c.preprocess(roi,True,mode)
        index,score,_=c.infer(model,roi,True,mode)
        result[mode]=dict(shape=list(tensor.shape),minimum=float(tensor.min()),maximum=float(tensor.max()),
                          predicted=c.CLASSES[index],score=score)
        images.append((tensor[0,0] if mode=='raw' else tensor[0,0]*255).clip(0,255).astype('uint8'))
        names.append('%s=%s %.2f'%(mode,c.CLASSES[index],score))
    return result,c.montage(images,names,3)

if __name__ == '__main__':
    c.run_experiment(Path(__file__).resolve().parents[1].name, run)
