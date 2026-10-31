"""Small, reproducible CPU experiments. Generated data stays outside this repo."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import random

import cv2
import numpy as np
import torch
from torch import nn

ROOT = Path(__file__).resolve().parents[1]
RECORDS = Path(os.environ.get('NN_COURSE_RECORDS', str(Path.home() / 'catkin_ws/records/neural-network'))).expanduser()
MODELS = Path(os.environ.get('NN_COURSE_MODELS', str(RECORDS / 'reference-models'))).expanduser()
CLASSES = [str(i) for i in range(10)] + ['negative']
FONTS = [0, 2, 3, 4, 6, 5, 1, 7]
TRAIN_FONTS, VAL_FONTS, TEST_FONTS = [0, 2, 3, 4, 6], [5], [1, 7]
torch.set_num_threads(2)
cv2.setNumThreads(1)


def seed(value=731):
    random.seed(value)
    np.random.seed(value)
    torch.manual_seed(value)


def save_json(path, data):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(data, ensure_ascii=False, indent=2, allow_nan=False) + '\n')


def read_json(path):
    return json.loads(Path(path).read_text())


class TinyCNN(nn.Module):
    def __init__(self):
        super().__init__()
        self.features = nn.Sequential(nn.Conv2d(1, 8, 3, padding=1), nn.ReLU(), nn.MaxPool2d(2),
                                      nn.Conv2d(8, 16, 3, padding=1), nn.ReLU(), nn.MaxPool2d(2))
        self.classifier = nn.Sequential(nn.Flatten(1), nn.Linear(16 * 8 * 8, 48), nn.ReLU(),
                                        nn.Dropout(0.25), nn.Linear(48, len(CLASSES)))

    def forward(self, image):
        return self.classifier(self.features(image))  # Raw logits; CrossEntropyLoss applies log-softmax.


def glyph(label, font, rng, augment=True):
    image = np.zeros((32, 32), np.uint8)
    if label == 10:
        choice = int(rng.integers(3))
        if choice == 1:
            image[:] = rng.integers(0, 30, (32, 32), dtype=np.uint8)
        elif choice == 2:
            for _ in range(3):
                p = tuple(int(x) for x in rng.integers(0, 32, 2))
                q = tuple(int(x) for x in rng.integers(0, 32, 2))
                cv2.line(image, p, q, int(rng.integers(30, 130)), 1)
        return image
    text = str(label)
    thickness = int(rng.integers(1, 3))
    (_, height), _ = cv2.getTextSize(text, font, 1, thickness)
    scale = float(rng.uniform(21, 27)) / max(1, height)
    (width, height), _ = cv2.getTextSize(text, font, scale, thickness)
    dx, dy = (rng.integers(-2, 3, 2) if augment else (0, 0))
    cv2.putText(image, text, ((32 - width) // 2 + int(dx), (32 + height) // 2 + int(dy)),
                font, scale, 235, thickness, cv2.LINE_AA)
    if augment:
        rotation = cv2.getRotationMatrix2D((15.5, 15.5), float(rng.uniform(-12, 12)), 1)
        image = cv2.warpAffine(image, rotation, (32, 32))
        image = np.clip(image.astype(np.float32) * rng.uniform(.65, 1.05) + rng.uniform(0, 18), 0, 255).astype(np.uint8)
        if rng.random() < .25:
            image = cv2.GaussianBlur(image, (3, 3), .6)
    return image


def prepare_data(per_class=90):
    destination = RECORDS / 'assets'
    destination.mkdir(parents=True, exist_ok=True)
    data_path = destination / 'digits-v1.npz'
    if data_path.exists():
        return data_path
    rng = np.random.default_rng(731)
    images, labels, groups = [], [], []
    for font in FONTS:
        for label in range(len(CLASSES)):
            for _ in range(per_class):
                images.append(glyph(label, font, rng))
                labels.append(label)
                groups.append(font)
    np.savez_compressed(data_path, images=np.stack(images), labels=np.array(labels), groups=np.array(groups))
    save_json(destination / 'dataset.json', {
        'source': 'PROGRAM_GENERATED_HERSHEY_DIGITS', 'seed': 731, 'per_font_class': per_class,
        'classes': CLASSES, 'train_fonts': TRAIN_FONTS, 'validation_fonts': VAL_FONTS,
        'test_fonts': TEST_FONTS, 'split_before_training_augmentation': True,
        'opencv_version': cv2.__version__, 'samples': len(labels), 'size': [32, 32]})
    board = np.zeros((11 * 48, 8 * 48, 3), np.uint8)
    for column, font in enumerate(FONTS):
        for row in range(11):
            gray = glyph(row, font, np.random.default_rng(100 + row), False)
            board[row * 48:row * 48 + 32, column * 48:column * 48 + 32] = cv2.cvtColor(gray, cv2.COLOR_GRAY2BGR)
    cv2.imwrite(str(destination / 'dataset-preview.png'), board)
    return data_path


def dataset(split='grouped'):
    file = np.load(prepare_data())
    images, labels, groups = file['images'], file['labels'], file['groups']
    if split == 'grouped':
        masks = [np.isin(groups, g) for g in (TRAIN_FONTS, VAL_FONTS, TEST_FONTS)]
        indices = [np.flatnonzero(mask) for mask in masks]
    elif split == 'random':
        order = np.random.default_rng(731).permutation(len(labels))
        a, b = int(len(order) * 5 / 8), int(len(order) * 6 / 8)
        indices = [order[:a], order[a:b], order[b:]]
    else:
        raise ValueError('split must be grouped or random')
    return images, labels, groups, indices


def tensors(images, labels):
    return torch.from_numpy(images.copy()).float().unsqueeze(1) / 255, torch.from_numpy(labels.copy()).long()


def load_model(path=None):
    model = TinyCNN()
    state = torch.load(str(path or MODELS / 'baseline.pt'), map_location='cpu', weights_only=True)
    model.load_state_dict(state)
    return model.eval()


def layer_shapes(model):
    """Record actual tensor shapes during one evaluation forward pass."""
    shapes, handles = [], []
    for name, module in model.named_modules():
        if name and not list(module.children()):
            def record(module, inputs, output, name=name):
                shapes.append({'layer': name, 'type': type(module).__name__,
                               'input': list(inputs[0].shape), 'output': list(output.shape)})
            handles.append(module.register_forward_hook(record))
    model.eval()
    with torch.no_grad():
        model(torch.zeros(1, 1, 32, 32))
    for handle in handles:
        handle.remove()
    return shapes


def evaluate(model, images, labels):
    model.eval()
    x, y = tensors(images, labels)
    outputs = []
    with torch.no_grad():
        for batch in x.split(128):
            outputs.append(model(batch))
    logits = torch.cat(outputs)
    prediction = logits.argmax(1)
    confusion = np.zeros((len(CLASSES), len(CLASSES)), int)
    for actual, predicted in zip(labels, prediction.numpy()):
        confusion[int(actual), int(predicted)] += 1
    return {'accuracy': float((prediction == y).float().mean()),
            'loss': float(nn.functional.cross_entropy(logits, y)), 'confusion': confusion.tolist(),
            'logits': logits.numpy(), 'prediction': prediction.numpy()}


def train(output, epochs=12, lr=.001, split='grouped', limit=0, augment=False):
    seed()
    images, labels, _, indices = dataset(split)
    ti, vi, _ = indices
    if limit:
        ti = np.random.default_rng(731).permutation(ti)[:limit]
    x, y = tensors(images[ti], labels[ti])
    loader = torch.utils.data.DataLoader(torch.utils.data.TensorDataset(x, y), batch_size=128, shuffle=True)
    model = TinyCNN()
    optimizer = torch.optim.Adam(model.parameters(), lr=lr)
    history, best, best_state, best_epoch = [], -1, None, 0
    for epoch in range(epochs):
        model.train()
        losses = []
        for bx, by in loader:
            if augment:
                bx = torch.clamp(bx * (.65 + .5 * torch.rand(len(bx), 1, 1, 1)), 0, 1)
                if random.random() < .5:
                    bx = nn.functional.avg_pool2d(bx, 3, 1, 1)
            optimizer.zero_grad()
            loss = nn.functional.cross_entropy(model(bx), by)
            loss.backward()
            optimizer.step()
            losses.append(float(loss.detach()))
        tr = evaluate(model, images[ti], labels[ti])
        va = evaluate(model, images[vi], labels[vi])
        history.append({'epoch': epoch + 1, 'train_loss': float(np.mean(losses)),
                        'train_accuracy': tr['accuracy'], 'validation_accuracy': va['accuracy'],
                        'validation_loss': va['loss']})
        if va['accuracy'] > best:
            best = va['accuracy']
            best_epoch = epoch + 1
            best_state = {k: v.detach().clone() for k, v in model.state_dict().items()}
        print('epoch=%d train=%.3f validation=%.3f loss=%.3f' %
              (epoch + 1, tr['accuracy'], va['accuracy'], np.mean(losses)), flush=True)
    model.load_state_dict(best_state)
    output = Path(output)
    output.mkdir(parents=True, exist_ok=True)
    torch.save(best_state, output / 'model.pt')
    report = {'seed': 731, 'epochs': epochs, 'learning_rate': lr, 'split': split, 'augment': augment,
              'training_samples': len(ti), 'validation_samples': len(vi), 'history': history,
              'checkpoint_selection': 'best_validation_accuracy', 'best_epoch': best_epoch,
              'layer_shapes': layer_shapes(model),
              'parameters': sum(p.numel() for p in model.parameters())}
    save_json(output / 'training.json', report)
    draw_curves(history, output / 'curves.png')
    return model, report


def draw_curves(history, path):
    canvas = np.full((400, 720, 3), 245, np.uint8)
    cv2.putText(canvas, 'Accuracy: train green / validation red', (24, 28), cv2.FONT_HERSHEY_SIMPLEX, .65, (30, 30, 30), 1)
    cv2.rectangle(canvas, (50, 50), (690, 350), (100, 100, 100), 1)
    for key, color in [('train_accuracy', (0, 150, 0)), ('validation_accuracy', (0, 0, 200))]:
        points = [(50 + round(i * 640 / max(1, len(history)-1)), 350 - round(row[key] * 300)) for i, row in enumerate(history)]
        for a, b in zip(points, points[1:]):
            cv2.line(canvas, a, b, color, 2)
    cv2.imwrite(str(path), canvas)


def preprocess(image, is_roi=False, mode='normal'):
    if is_roi:
        width = image.shape[1]
        image = image[:, width // 4:3 * width // 4]
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY) if image.ndim == 3 else image
    gray = cv2.resize(gray, (32, 32), interpolation=cv2.INTER_AREA)
    if mode == 'invert':
        gray = 255 - gray
    value = gray.astype(np.float32)
    if mode != 'raw':
        value /= 255
    return value[None, None]


def infer(model, image, is_roi=False, mode='normal'):
    with torch.no_grad():
        logits = model(torch.from_numpy(preprocess(image, is_roi, mode)))
        probabilities = logits.softmax(1)[0].numpy()
    index = int(probabilities.argmax())
    return index, float(probabilities[index]), logits[0].numpy()


def montage(images, labels, columns=6):
    rows = (len(images) + columns - 1) // columns
    canvas = np.full((rows * 112, columns * 128, 3), 25, np.uint8)
    for index, (image, label) in enumerate(zip(images, labels)):
        image = cv2.cvtColor(image, cv2.COLOR_GRAY2BGR) if image.ndim == 2 else image
        resized = cv2.resize(image, (88, 80), interpolation=cv2.INTER_NEAREST)
        x, y = index % columns * 128, index // columns * 112
        canvas[y:y+80, x:x+88] = resized
        cv2.putText(canvas, str(label)[:18], (x+2, y+100), cv2.FONT_HERSHEY_SIMPLEX, .4, (230, 230, 230), 1)
    return canvas


def show(image, title, headless=False, output=None):
    if output:
        cv2.imwrite(str(Path(output) / 'overview.png'), image)
    if headless:
        return
    cv2.namedWindow(title, cv2.WINDOW_NORMAL | cv2.WINDOW_KEEPRATIO)
    cv2.imshow(title, image)
    while True:
        key = cv2.waitKey(30) & 255
        if cv2.getWindowProperty(title, cv2.WND_PROP_AUTOSIZE) < 0:
            return  # GTK has already destroyed a window closed with its title-bar button.
        if key in (27, ord('q'), ord('Q')):
            break
    cv2.destroyWindow(title)


def export_model(model, output):
    import onnx
    output = Path(output)
    output.parent.mkdir(parents=True, exist_ok=True)
    model.eval()
    torch.onnx.export(model, torch.zeros(1, 1, 32, 32), str(output), opset_version=11,
                      input_names=['image'], output_names=['logits'], do_constant_folding=True)
    graph = onnx.load(str(output))
    onnx.checker.check_model(graph)
    return sorted(set(node.op_type for node in graph.graph.node))


def prepare_video():
    folder = RECORDS / 'assets/frames'
    if (folder / 'frame-179.png').exists():
        return
    folder.mkdir(parents=True, exist_ok=True)
    K = np.array([[800, 0, 320], [0, 800, 240], [0, 0, 1]], float)
    points = np.array([[-.075,-.06,0],[.075,-.06,0],[.075,.06,0],[-.075,.06,0]], np.float32)
    destination = np.array([[0,0],[159,0],[159,79],[0,79]], np.float32)
    truth = []
    for index in range(180):
        phase = 2*np.pi*index/180
        rv = np.array([.025*np.sin(phase), .07*np.cos(phase), .12*np.sin(phase)])
        tv = np.array([.18*np.sin(phase), .065*np.cos(phase), 1.65+.25*np.sin(phase)])
        quad = cv2.projectPoints(points, rv, tv, K, None)[0].reshape(4,2)
        image = np.full((480,640,3), 20, np.uint8)
        for x in (-.075, .075):
            bar = np.array([[x-.007,-.06,0],[x+.007,-.06,0],[x+.007,.06,0],[x-.007,.06,0]], np.float32)
            projected = cv2.projectPoints(bar, rv, tv, K, None)[0].reshape(4,2)
            cv2.fillConvexPoly(image, np.rint(projected).astype(np.int32), (20,20,240))
        digit = index // 18
        raw = glyph(digit, 0, np.random.default_rng(100+digit), False)
        pattern = np.zeros((80,160), np.uint8)
        pattern[:,40:120] = cv2.resize(raw, (80,80))
        warped = cv2.warpPerspective(pattern, cv2.getPerspectiveTransform(destination, quad), (640,480))
        image[warped > 30] = warped[warped > 30, None]
        cv2.rectangle(image, (45,60),(90,105),(25,25,180),-1)
        cv2.rectangle(image, (540,330),(556,430),(220,30,20),-1)
        visible = not 80 <= index < 95
        if not visible:
            right = (quad[1]+quad[2])/2
            cv2.rectangle(image, (int(right[0]-13),int(right[1]-60)), (int(right[0]+14),int(right[1]+60)),(20,20,20),-1)
        cv2.imwrite(str(folder / ('frame-%03d.png' % index)), image)
        truth.append({'frame':index,'digit':digit,'visible':visible,'z_m':float(tv[2])})
    save_json(RECORDS / 'assets/video-truth.json', truth)


def bundle(epochs=14):
    MODELS.mkdir(parents=True, exist_ok=True)
    model, training = train(RECORDS/'baseline-training', epochs=epochs, augment=True)
    torch.save(model.state_dict(), MODELS/'baseline.pt')
    ops = export_model(model, MODELS/'baseline.onnx')
    images, labels, _, indices = dataset()
    report = {}
    for name, ids in zip(('train','validation','test'), indices):
        result = evaluate(model, images[ids], labels[ids])
        report[name] = {k:v for k,v in result.items() if k not in ('logits','prediction')}
    save_json(MODELS/'baseline-evaluation.json', report)
    save_json(MODELS/'training-history.json', training)
    save_json(MODELS/'metadata.json', {'classes':CLASSES,'negative_id':10,'input_shape':[1,1,32,32],
        'scale':1/255,'color':'gray','roi_crop':'middle_half_width','threshold':.65,
        'onnx_opset':11,'operators':ops,'torch':torch.__version__, 'training_source':'PROGRAM_GENERATED_HERSHEY_DIGITS',
        'seed':731,'training_data':read_json(RECORDS/'assets/dataset.json'),
        'training_dataset_sha256':hashlib.sha256((RECORDS/'assets/digits-v1.npz').read_bytes()).hexdigest(),
        'sha256':hashlib.sha256((MODELS/'baseline.onnx').read_bytes()).hexdigest()})
    fixtures = MODELS/'fixtures'
    fixtures.mkdir(exist_ok=True)
    cases = []
    for i in range(11):
        image = glyph(i, 0, np.random.default_rng(10+i), False)
        path = fixtures / ('digit-%02d.png' % i)
        cv2.imwrite(str(path), image)
        _, _, logits = infer(model, image)
        cases.append({'image':str(path.relative_to(MODELS)), 'roi':0, 'logits':logits.tolist()})
    roi = np.zeros((80,160,3),np.uint8)
    roi[:,40:120] = cv2.cvtColor(cv2.resize(glyph(7,0,np.random.default_rng(17),False),(80,80)),cv2.COLOR_GRAY2BGR)
    cv2.rectangle(roi,(3,2),(10,77),(20,20,240),-1)
    cv2.rectangle(roi,(149,2),(156,77),(20,20,240),-1)
    cv2.imwrite(str(fixtures/'roi-7.png'), roi)
    cases.append({'image':'fixtures/roi-7.png','roi':1,'logits':infer(model,roi,True)[2].tolist()})
    save_json(MODELS/'parity.json', {'absolute_tolerance':.001,'cases':cases})
    prepare_video()
    print('BASELINE READY', {k:round(v['accuracy'],3) for k,v in report.items()}, ops, flush=True)


def options():
    parser = argparse.ArgumentParser()
    parser.add_argument('--headless', action='store_true')
    parser.add_argument('--epochs', type=int, default=5)
    parser.add_argument('--lr', type=float, default=.001)
    parser.add_argument('--limit', type=int, default=0)
    parser.add_argument('--augment', action='store_true')
    parser.add_argument('--threshold', type=float, default=.65)
    parser.add_argument('--split', choices=['grouped','random'], default='grouped')
    parser.add_argument('--mode', default='normal')
    parser.add_argument('--chapter', default='')
    parser.add_argument('--output', default='')
    parser.add_argument('--checkpoint', default='')
    parser.add_argument('--models', default='', help='chapter 10 deployment model package directory')
    parser.add_argument('--class-id', type=int, choices=range(-1, len(CLASSES)), default=-1,
                        help='chapter 05 display filter: -1=all, 0..9=digits, 10=negative')
    args = parser.parse_args()
    if args.epochs < 1 or args.lr <= 0 or args.limit < 0 or not 0 <= args.threshold <= 1:
        parser.error('epochs/lr must be positive; limit nonnegative; threshold within [0,1]')
    return args


def run_experiment(folder, experiment):
    args = options()
    args.output = Path(args.output) if args.output else RECORDS / folder
    args.output.mkdir(parents=True, exist_ok=True)
    seed()
    prepare_data()
    result, image = experiment(args)
    save_json(args.output/'metrics.json', result)
    print(json.dumps(result, ensure_ascii=False, default=str), flush=True)
    show(image, folder, args.headless, args.output)


if __name__ == '__main__':
    args = options()
    if args.chapter == 'bundle':
        bundle(args.epochs)
    elif args.chapter == 'prepare':
        prepare_data(); prepare_video()
    else:
        raise SystemExit('Run a numbered chapter src/main.py, or use --chapter=prepare/bundle')
