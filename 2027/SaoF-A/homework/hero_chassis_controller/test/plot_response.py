#!/usr/bin/env python3
"""Plot recorded wheel feedback from verify_simulation.py."""
import argparse
import json
from pathlib import Path
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('evidence')
parser.add_argument('output')
args = parser.parse_args()
data = json.loads(Path(args.evidence).read_text())
rows = [x for x in data['samples'] if x['phase'] in ['forward', 'forward_stop']]
if not rows:
    raise SystemExit('No forward-response samples found')
t0 = rows[0]['t']
fig, axes = plt.subplots(2, 2, figsize=(10, 6), sharex=True, sharey=True)
for i, (ax, label) in enumerate(zip(axes.flat, ['Front left', 'Front right', 'Back left', 'Back right'])):
    ax.plot([r['t'] - t0 for r in rows], [r['target'][i] for r in rows],
            label='Target', color='#195ca8', linestyle='--', linewidth=1.7)
    ax.plot([r['t'] - t0 for r in rows], [r['measured'][i] for r in rows],
            label='Measured', color='#c26013', linewidth=1.0)
    ax.set_title(label)
    ax.set_xlabel('Simulation time since command (s)')
    ax.set_ylabel('Wheel speed (rad/s)')
    ax.grid(alpha=0.2)
    ax.legend(loc='upper right')
fig.suptitle('Response to vx = 0.2 m/s, then stop (acceleration limit 0.5 m/s²)\n'
             'P = 0.8, I = 0.1, D = 0.001; effort limit = 5 N·m')
fig.tight_layout(rect=[0, 0, 1, 0.91])
fig.savefig(args.output, dpi=180)
