import numpy as np
import matplotlib.pyplot as plt

def wgn(signal, snr = 0):
    snr = 10 ** (snr / 10)
    xpower = np.sum(signal ** 2) / len(signal)
    npower = xpower / snr
    noise = np.random.randn(len(signal)) * np.sqrt(npower)
    return noise

def hilbert(signal, num_hilbert = 11):
    hilbert_transformer = np.zeros(num_hilbert, dtype=np.complex128)
    for i in range(-num_hilbert // 2, num_hilbert // 2 + 1, 1):
        if i % 2 == 0:
            hilbert_transformer[i + num_hilbert // 2] = 0
        else:
            hilbert_transformer[i + num_hilbert // 2] = 2j / (np.pi * i)

    hilbert_transformer[0 + num_hilbert // 2] = 1
    
    signal_hilbert = np.convolve(signal, hilbert_transformer, mode='same')
    
    return signal_hilbert

def pulse_compress(signal_src, signal_ref):
    signal_src_length = len(signal_src)
    signal_ref_length = len(signal_ref)
    

    fft_len = 2 * signal_src_length - 1
    fft_len = np.ceil(0.5 + np.log2(fft_len))
    fft_len = np.floor(0.5 + 2 ** fft_len)
    
    signal_ref = np.conjugate(signal_ref)
    signal_ref = np.flip(signal_ref)
    
    window = np.hamming(signal_ref_length)
    # signal_ref = signal_ref * window

    signal_src = np.pad(signal_src, (0, int(fft_len - signal_src_length)), 'constant')
    signal_ref = np.pad(signal_ref, (0, int(fft_len - signal_ref_length)), 'constant')
    
    signal_src_fft = np.fft.fft(signal_src, fft_len)
    signal_ref_fft = np.fft.fft(signal_ref, fft_len)
    
    signal_dst_fft = signal_src_fft * signal_ref_fft
    
    signal_dst = np.fft.ifft(signal_dst_fft, fft_len)
    
    return signal_dst

def lfm(tau, fs, fl, bw):
    times = np.linspace(0, tau - 1 / fs, int(tau * fs))
    phase = 2 * np.pi * fl * times + np.pi * bw / tau * times ** 2
    signal = np.exp(1j * phase)
    
    return signal

signal_ref = lfm(7e-6, 20e6, 222e6, 6e6)
signal = np.real(lfm(7e-6, 20e6, 222e6, 6e6))
signal_hilbert = hilbert(signal, 11)
signal_compressed = pulse_compress(signal_hilbert, signal_ref)
# plt.plot(np.abs(signal_compressed), label='radar-signal-compressed-abs')
# generate a lot of wgn from lfm and overlap them in graph to see the noise distribution
# for i in range(100):
#     noise = wgn(signal, 0)
#     noise_hilbert = hilbert(noise)
#     noise_compressed = pulse_compress(noise_hilbert, signal_ref)
    
#     # scatter of time and abs
#     plt.scatter(np.arange(len(noise_compressed)), np.abs(noise_compressed), color='red', s=0.1, label='noise-compressed-abs')
    

fft_len = 2 * len(signal) - 1
fft_len = np.ceil(0.5 + np.log2(fft_len))
fft_len = np.floor(0.5 + 2 ** fft_len)
    
noise = wgn(signal, 0)
noise_hilbert = hilbert(noise)
noise_compressed = pulse_compress(noise_hilbert, signal_ref)

noise_pad = np.pad(noise_hilbert, (0, int(fft_len - len(noise_hilbert))), 'constant')
noise_fft = np.fft.fft(noise_pad, fft_len)

signal_ref_pad = np.pad(signal_ref, (0, int(fft_len - len(signal_ref))), 'constant')
signal_ref_fft = np.fft.fft(signal_ref_pad, fft_len)

noise_compressed_fft = np.fft.fft(noise_compressed, fft_len)


# plt.plot(np.real(noise_fft), label = 'noise-fft-real')
# plt.plot(np.imag(noise_fft), label = 'noise-fft-imag')
plt.plot(np.abs(noise_fft), label = 'noise-fft-abs')
plt.plot(np.abs(noise_compressed_fft), label = 'noise-compressed-fft-abs')
plt.plot(np.abs(signal_ref_fft), label = 'signal-ref-fft-abs')
plt.legend()

plt.savefig('./data/noise_distribution.png')


