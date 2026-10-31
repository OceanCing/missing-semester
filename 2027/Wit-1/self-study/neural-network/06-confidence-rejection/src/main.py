from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / '_shared'))

import cv2
import numpy as np
import core as c


def run(args):
    model=c.load_model(args.checkpoint or None)
    images=[c.glyph(i,0,np.random.default_rng(23+i),False) for i in range(11)]
    actual=[str(i) for i in range(10)]+['negative']
    for letter in ('A','X','?','B','S'):
        image=np.zeros((32,32),np.uint8)
        cv2.putText(image,letter,(3,27),cv2.FONT_HERSHEY_SIMPLEX,.9,235,2,cv2.LINE_AA)
        images.append(image);actual.append('OOD-'+letter)
    records=[]
    for image,expected in zip(images,actual):
        index,score,_=c.infer(model,image)
        records.append(dict(input=expected,predicted=c.CLASSES[index],score=score,
                            accepted=index!=10 and score>=args.threshold))
    sweep={str(t):sum(r['predicted']!='negative' and r['score']>=t for r in records) for t in (.2,.65,.95)}
    c.save_json(args.output/'predictions.json',records)
    captions=['%s->%s %s'%(r['input'],r['predicted'],'OK' if r['accepted'] else 'REJECT') for r in records]
    return dict(threshold=args.threshold,acceptance_counts=sweep,predictions=records),c.montage(images,captions)

if __name__ == '__main__':
    c.run_experiment(Path(__file__).resolve().parents[1].name, run)
