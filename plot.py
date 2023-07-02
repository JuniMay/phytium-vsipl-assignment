import matplotlib.pyplot as plt
import numpy as np


radar_signal = list(map(float, open('./data/radar_signal.txt', 'r').readlines()))

radar_signal_filtered = open('./data/radar_signal_filtered.txt', 'r').readlines()
radar_signal_filtered = map(lambda x: x.replace(' ', '').replace('\n', ''), radar_signal_filtered)
radar_signal_filtered = list(map(complex, radar_signal_filtered))

radar_signal_compressed = open('./data/radar_signal_compressed.txt', 'r').readlines()
radar_signal_compressed = map(lambda x: x.replace(' ', '').replace('\n', ''), radar_signal_compressed)
radar_signal_compressed = list(map(complex, radar_signal_compressed))

hilbert_coefficients = open('./data/hilbert_coefficients.txt', 'r').readlines()
hilbert_coefficients = map(lambda x: x.replace(' ', '').replace('\n', ''), hilbert_coefficients)
hilbert_coefficients = list(map(complex, hilbert_coefficients))

reference_signal = open('./data/reference_signal.txt', 'r').readlines()
reference_signal = map(lambda x: x.replace(' ', '').replace('\n', ''), reference_signal)
reference_signal = list(map(complex, reference_signal))

plt.plot(radar_signal, label='radar-signal')
plt.plot(np.abs(radar_signal_filtered), label='radar-signal-filtered')
plt.legend()
plt.savefig('./data/radar_signal_filtered.png')

plt.clf()

plt.plot(np.abs(radar_signal_compressed), label='radar-signal-compressed-abs')
plt.legend()
plt.savefig('./data/radar_signal_compressed.png')

plt.clf()

plt.plot(np.real(hilbert_coefficients), label='hilbert-coefficients-real', marker='o', mfc='w')
plt.plot(np.imag(hilbert_coefficients), label='hilbert-coefficients-imag', marker='o', mfc='w')
plt.legend()
plt.savefig('./data/hilbert_coefficients.png')

plt.clf()

plt.plot(np.real(reference_signal), label='reference-signal-real')
plt.legend()
plt.savefig('./data/reference_signal.png')

plt.clf()
