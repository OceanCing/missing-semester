from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / '_shared'))

import cv2
import numpy as np
import torch
from torch import nn
import core as c


def run(args):
    images, labels, _, ids = c.dataset()
    x, y = c.tensors(images[ids[0][:32]], labels[ids[0][:32]])
    model = c.load_model()
    model.train()
    a, b = model(x), model(x)
    dropout_changes = not torch.allclose(a, b)
    model.eval()
    a, b = model(x), model(x)
    eval_stable = torch.allclose(a, b)
    eval_tracks_grad = a.requires_grad
    with torch.no_grad():
        no_grad_tracks = model(x).requires_grad
    before = [p.detach().clone() for p in model.parameters()]
    loss = nn.functional.cross_entropy(model(x), y)
    loss.backward()
    after_backward = max(float((p-v).abs().max()) for p,v in zip(model.parameters(),before))
    gradients = [p.grad.detach().clone() for p in model.parameters()]
    optimizer = torch.optim.SGD(model.parameters(), lr=args.lr)
    optimizer.step()
    after_step = max(float((p-v).abs().max()) for p,v in zip(model.parameters(),before))
    other = c.load_model()
    nn.functional.cross_entropy(other(x),y).backward()
    gradient_same = all(torch.allclose(p.grad,g) for p,g in zip(other.parameters(),gradients))
    result = dict(dropout_changes=dropout_changes, eval_stable=bool(eval_stable),
                  eval_still_records_grad=bool(eval_tracks_grad), no_grad_records_grad=bool(no_grad_tracks),
                  weight_change_after_backward=after_backward, weight_change_after_step=after_step,
                  same_gradient_before_lr_is_used=gradient_same, learning_rate=args.lr)
    canvas = np.full((390,850,3),25,np.uint8)
    for i,(key,value) in enumerate(result.items()):
        cv2.putText(canvas, '%s: %s' % (key,value), (20,40+i*38),cv2.FONT_HERSHEY_SIMPLEX,.54,(230,230,230),1)
    return result, canvas

if __name__ == '__main__':
    c.run_experiment(Path(__file__).resolve().parents[1].name, run)
