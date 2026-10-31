from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / '_shared'))

import core as c
import numpy as np


def run(args):
    images, labels, groups, grouped = c.dataset()
    _, _, _, random = c.dataset('random')
    result = {}
    for name, splits in [('grouped',grouped),('random',random)]:
        sources = [set(groups[i].tolist()) for i in splits]
        result[name] = dict(samples=[len(i) for i in splits],
                            train_sources=sorted(sources[0]), validation_sources=sorted(sources[1]),
                            test_sources=sorted(sources[2]),
                            train_validation_overlap=sorted(sources[0] & sources[1]))
        if args.mode == 'train-compare':
            model, report = c.train(args.output/name,args.epochs,args.lr,split=name)
            vi=splits[1]
            result[name]['validation_accuracy'] = c.evaluate(model,images[vi],labels[vi])['accuracy']
    # Blank negative examples deliberately repeat. Group separation alone is not a duplicate audit.
    training_pixels = {images[i].tobytes() for i in grouped[0]}
    duplicates = [int(i) for i in grouped[1] if images[i].tobytes() in training_pixels]
    result['exact_duplicate_audit'] = dict(
        validation_samples_matching_training=len(duplicates),
        digit_samples_matching_training=sum(int(labels[i]) != 10 for i in duplicates),
        negative_samples_matching_training=sum(int(labels[i]) == 10 for i in duplicates),
        caveat='Synthetic source groups are a teaching split, not independent real captures.')
    selected = [next(int(i) for i in part if labels[i] == 7) for part in grouped]
    selected += [int(i) for i in random[1][:9]]
    return result, c.montage([images[i] for i in selected],['label=%s font=%s'%(labels[i],groups[i]) for i in selected])

if __name__ == '__main__':
    c.run_experiment(Path(__file__).resolve().parents[1].name, run)
