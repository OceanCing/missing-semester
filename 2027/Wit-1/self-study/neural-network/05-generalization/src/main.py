from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / '_shared'))

import cv2
import numpy as np
import core as c


def run(args):
    model = c.load_model(args.checkpoint or None)
    images,labels,_,splits=c.dataset()
    ids=splits[2] if args.mode=='test' else splits[1]
    selected=images[ids].copy()
    if args.mode=='blur':
        selected=np.stack([cv2.GaussianBlur(x,(7,7),2) for x in selected])
    if args.mode=='dark':
        selected=(selected.astype(np.float32)*.2).astype(np.uint8)
    evaluated=c.evaluate(model,selected,labels[ids])
    c.save_json(args.output/'confusion.json',evaluated['confusion'])
    display_mask=np.ones(len(ids),dtype=bool) if args.class_id<0 else labels[ids]==args.class_id
    wrong=np.flatnonzero((evaluated['prediction'] != labels[ids]) & display_mask)
    filtered_errors=len(wrong)
    wrong=wrong[:24]
    if not len(wrong):
        wrong=np.flatnonzero(display_mask)[:12]
    captions=['true=%s pred=%s'%(c.CLASSES[labels[ids[i]]],c.CLASSES[evaluated['prediction'][i]]) for i in wrong]
    result={k:v for k,v in evaluated.items() if k not in ('logits','prediction')}
    result.update(mode=args.mode, evaluation_split='test' if args.mode=='test' else 'validation',samples=len(ids))
    result.update(shown_true_class='all' if args.class_id<0 else c.CLASSES[args.class_id],
                  filtered_errors=filtered_errors,displayed_examples=len(wrong),
                  display_mode='errors' if filtered_errors else 'examples_no_errors')
    return result,c.montage(selected[wrong],captions)

if __name__ == '__main__':
    c.run_experiment(Path(__file__).resolve().parents[1].name, run)
