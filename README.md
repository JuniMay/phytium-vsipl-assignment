# 基于飞腾平台的远距离探测系统

## 项目内容

项目基于 VISPL 函数库实现以下内容

### 封装汉明窗（Hamming Window）

$$
w(n)=a_0-(1-a_0)\cos\left(\frac{2\pi n}{N-1}\right), \quad 0\leq n\leq N-1
$$

### 利用余弦函数构造一个雷达回波脉冲（实信号）

线性调频信号的相位可以表示为：

$$
\varphi(t)=2\pi f_{\text{low}}t+\frac{\pi\text{BW}}{\tau}t^2
$$

线性调频的复信号则可以用欧拉公式表示为：

$$
s(t) = e^{i\varphi(t)}
$$

由于雷达回波得到的是实信号，所以我们只需要取复信号的实部即可，即应用余弦函数

$$
s_{\text{real}}(t) = \cos\left(2\pi f_{\text{low}}t+\frac{\pi\text{BW}}{\tau}t^2\right)
$$

设两个物体相聚为 $d$，则两个物体的回波相差的时间为

$$
\Delta t = \frac{2d}{c}
$$

其中 $c$ 为光速， $d$ 为两个物体的距离。假设接收到第一个物体反射信号的时间为 $t_0$，则雷达接收到的两个物体的信号分别为

$$
\begin{aligned}
s_1(t) &= \cos\left(2\pi f_{\text{low}}(t-t_0)+\frac{\pi\text{BW}}{\tau}(t-t_0)^2\right)\\
s_2(t) &= \cos\left(2\pi f_{\text{low}}(t-t_0-\Delta t)+\frac{\pi\text{BW}}{\tau}(t-t_0-\Delta t)^2\right)
\end{aligned}
$$

叠加的信号为

$$
s(t) = s_1(t)+s_2(t)
$$

### 在回波上叠加使其信噪比为 0dB 的高斯白噪声

信噪比的定义如下（单位为分贝）：

$$
\text{SNR} = 10\log_{10}\frac{P_{\text{signal}}}{P_{\text{noise}}}
$$

对于离散的采样，信噪比可以表示为：

$$
\text{SNR} = 10\log_{10}\frac{\sum_{i=0}^{N-1}x_i^2}{\sum_{i=0}^{N-1}n_i^2}
$$

其中 $x_i$ 为信号，$n_i$ 为噪声，且振幅满足分布：

$$
n \sim \mathcal{N}\left(0, \frac{\sum x_i^2}{10^{\left(\frac{\text{SNR}}{10}\right)}N} \right)
$$

### 设计希尔伯特滤波器

定义符号函数：

$$
\text{sgn}(x)=\begin{cases}
1, & x>0\\
0, & x=0\\
-1, & x<0
\end{cases}
$$

对于时域信号 $x(t)$，设其频域表示为 $X(\Omega)$，则在频域下的希尔伯特变换可以表示为：

$$
\widehat{X}(\Omega) = [-j\text{sgn}(\Omega)]X(\Omega)
$$

假设希尔伯特变换之后信号的时域表示为 $\widehat{x}(t)$，则希尔伯特滤波结果的时域表示为：

$$
s(t) = x(t)+j\widehat{x}(t)
$$

其频域表示为：

$$
\begin{aligned}
S(\Omega) &= X(\Omega)+j\widehat{X}(\Omega)\\
            &= X(\Omega)+j[-j\text{sgn}(\Omega)]X(\Omega)\\
            &= [1+\text{sgn}(\Omega)]X(\Omega)
\end{aligned}
$$

则希尔伯特滤波器的频域表示为：

$$
H(\Omega) = 1+\text{sgn}(\Omega)
$$

表示为时域下的卷积运算：

$$
s(t) = h(t) * x(t)
$$

其中 $h(t)$ 为希尔伯特滤波器 $H(\Omega)$ 的时域表示，通过逆傅里叶变换可以得到：

$$
\begin{aligned}
h(t) &= \mathcal{F}^{-1}\{H(\Omega)\}\\
        &= \mathcal{F}^{-1}\{1+\text{sgn}(\Omega)\}\\
        &= \delta(t)+\frac{1}{\pi t}
\end{aligned}
$$

但是由于所采用的是离散时间，所以需要对 $H(\Omega)$ 做离散时间傅里叶逆变换，得到 $h(t)$ 的离散表示：

$$
\begin{aligned}
h(n) &= \frac{1}{2\pi}\int_{-\pi}^{\pi}[1+\text{sgn}(\omega)]e^{j\omega n}\mathrm{d}\omega \\
& = \frac{1}{2\pi}\int_{0}^{\pi}2e^{j\omega n}\mathrm{d}\omega
= \frac{1}{\pi}\int_{0}^{\pi}e^{j\omega n}\mathrm{d}\omega \\
& = \frac{1}{\pi}\frac{e^{j\pi n} - 1}{jn} = \frac{\cos\pi n - 1}{j\pi n} = \frac{j(1-\cos\pi n)}{\pi n}\\
& = \begin{cases}
1, & n=0\\
\frac{2j}{\pi n}, & n \text{ 为奇数} \\
0, & n \text{ 为偶数}
\end{cases}
\end{aligned}
$$

### 应用脉冲压缩

### 检测目标数量和间距

根据一定的阈值对脉冲压缩之后所得到的信号进行筛选，之后线性遍历信号中离散的点，依据对应的下标和采样率得到时间，从而得到目标的距离。首先根据采样率 $f_{\mathrm{sample}}$ 得到采样的间隔
$$
\Delta t_{\mathrm{sample}} = \frac{1}{f_{\mathrm{sample}}}
$$
假设检测到了两个相邻的峰值，且二者的下标相差 $n$，则二者的时间间隔为
$$
\Delta t = n\Delta t_{\mathrm{sample}}
$$
最后根据光速计算得到距离
$$
d = \frac{c\Delta t}{2}
$$

## 流程图与关键接口

### 流程图

```mermaid
flowchart 
  A --> O
  O[参考信号] --> F
  subgraph 回波信号生成
    A[生成信号] --> B[叠加高斯白噪声]
    B --> C[得到实信号]
  end
  subgraph 希尔伯特滤波
    C --> D[希尔伯特滤波]
    D --> E[得到复信号]
    E --> L
  end
  subgraph 脉冲压缩
    F[参考信号的共轭与翻转] --> G[应用汉明窗]
    G --> K
    H[计算傅里叶变换长度] -- "补零" --> K[参考信号]
    H -- "补零" --> L[回波信号]
    K -- "傅里叶变换" --> J[频域相乘]
    L -- "傅里叶变换" --> J
    J --> M[脉冲压缩结果]
  end
  M --> N[检测目标及距离]
  P[对噪声信号单独采样变换仿真得到阈值] --> N
```

### 关键接口

希尔伯特滤波

```c
/*
 * 内部接口：希尔伯特滤波
 * 参数：p_vector_src    -- 输入信号
 *      n_filter_length -- 滤波器长度
 *      p_vector_dst    -- 输出信号
 * 功能：对输入信号进行希尔伯特滤波
 */
void hilbert(vsip_vview_f *p_vector_src, vsip_scalar_i n_filter_length,
             vsip_cvview_f *p_vector_dst);

```

汉明窗

```c
/*
 * 内部接口：生成汉明窗
 * 参数：p_vector_dst -- 输出信号
 * 功能：根据输出信号的长度生成汉明窗
 */
void vcreate_hamming_f(vsip_vview_f *p_vector_dst);
```

信号生成以及处理

```c
/*
 * 内部接口：生成线性调频信号
 * 参数：f_tau           -- 脉冲宽度
 *      f_freq_sampling -- 采样频率
 *      f_freq_low      -- 起始频率
 *      f_band_width    -- 带宽
 *      p_vector_dst    -- 输出信号
 * 功能：生成线性调频信号（复信号）
 */
void generate_lfm_signal(vsip_scalar_f f_tau, vsip_scalar_f f_freq_sampling,
                         vsip_scalar_f f_freq_low, vsip_scalar_f f_band_width,
                         vsip_cvview_f *p_vector_dst);

/*
 * 内部接口：生成线性调频信号
 * 参数：f_tau           -- 脉冲宽度
 *      f_freq_sampling -- 采样频率
 *      f_freq_low      -- 起始频率
 *      f_band_width    -- 带宽
 *      p_vector_dst    -- 输出信号
 * 功能：生成线性调频信号（实信号）
 */
void generate_lfm_signal_real(vsip_scalar_f f_tau, vsip_scalar_f f_freq_sampling,
                              vsip_scalar_f f_freq_low, vsip_scalar_f f_band_width,
                              vsip_vview_f *p_vector_dst);

/*
 * 内部接口：生成雷达回波信号
 * 参数：f_tau           -- 脉冲宽度
 *      f_freq_sampling -- 采样频率
 *      f_freq_low      -- 起始频率
 *      f_band_width    -- 带宽
 *      f_disatance     -- 两个物体之间的距离
 *      p_vector_dst    -- 输出信号
 * 功能：生成两个有一定距离的物体反射叠加得到的雷达回波信号
 */
void generate_radar_signal(vsip_scalar_f f_tau, vsip_scalar_f f_freq_sampling,
                           vsip_scalar_f f_freq_low, vsip_scalar_f f_band_width,
                           vsip_scalar_f f_distance, vsip_vview_f *p_vector_dst);

/*
 * 内部接口：生成雷达回波信号
 * 参数：p_vector_signal -- 输入信号
 *      f_snr           -- 目标信号信噪比
 *      p_vector_dst    -- 输出信号
 * 功能：生成可以叠加到原信号上的给定信噪比的高斯白噪声
 */
void generate_wgn_signal(vsip_vview_f *p_vector_signal, vsip_scalar_f f_snr,
                         vsip_vview_f *p_vector_dst);

/*
 * 内部接口：脉冲压缩
 * 参数：p_vector_signal_src -- 输入信号
 *      p_vector_signal_ref -- 参考信号
 *      p_vector_dst        -- 输出信号
 * 功能：使用给定的参考信号对输入信号进行脉冲压缩
 */
void pulse_compress(vsip_cvview_f *p_vector_signal_src, vsip_cvview_f *p_vector_signal_ref,
                    vsip_cvview_f *p_vector_dst);


/*
 * 内部接口：检测信号
 * 参数：p_vector_signal -- 脉冲压缩之后得到的信号
 *      f_threshold     -- 阈值
 *      p_vector_dst    -- 输出信号
 * 功能：对脉冲压缩之后的信号进行进一步检测，依据阈值进行筛选
 */
void detect_signal(vsip_cvview_f *p_vector_signal, vsip_scalar_f f_threshold,
                   vsip_cvview_f *p_vector_dst);
```

用于输出和调试的函数

```c
/*
 * 内部接口：输出实向量
 * 参数：p_vector -- 输入向量
 *      p_file   -- 输出文件
 * 功能：将实向量的数据输出到文件
 */
void vdump_f(vsip_vview_f *p_vector, FILE *p_file);

/*
 * 内部接口：输出复向量
 * 参数：p_vector -- 输入向量
 *      p_file   -- 输出文件
 * 功能：将复向量的数据输出到文件
 */
void cvdump_f(vsip_cvview_f *p_vector, FILE *p_file);

/*
 * 内部接口：实向量调试
 * 参数：p_vector -- 输入向量
 *      p_name   -- 输出文件名
 * 功能：将实向量的数据输出到指定文件名的文件
 */
void vdebug_f(vsip_vview_f *p_vector, char *p_name);

/*
 * 内部接口：复向量调试
 * 参数：p_vector -- 输入向量
 *      p_name   -- 输出文件名
 * 功能：将复向量的数据输出到指定文件名的文件
 */
void cvdebug_f(vsip_cvview_f *p_vector, char *p_name);

/*
 * 内部接口：复向量翻转
 * 参数：p_vector_src -- 输入向量
 *      p_vector_dst -- 输出向量
 * 功能：将输入向量的数据翻转后输出到输出向量
 */
void cvflip_f(vsip_cvview_f *p_vector_src, vsip_cvview_f *p_vector_dst);

/*
 * 内部接口：复向量填充
 * 参数：p_vector_src -- 输入向量
 *      p_vector_dst -- 输出向量
 * 功能：根据输出向量的长度对输入向量进行零填充得到输出
 */
void cvpad_f(vsip_cvview_f *p_vector_src, vsip_cvview_f *p_vector_dst);

```

## 关于本项目

本项目为 NKU 2023 暑期实习实训飞腾课程 VSIPL 大作业。
