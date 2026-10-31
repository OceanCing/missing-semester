from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / '_shared'))

import cv2
import core as c


def run(args):
    model, report=c.train(args.output,args.epochs,args.lr,args.split,args.limit,args.augment)
    report['layers'] = str(model)
    return report,cv2.imread(str(args.output/'curves.png'))

if __name__ == '__main__':
    c.run_experiment(Path(__file__).resolve().parents[1].name, run)
