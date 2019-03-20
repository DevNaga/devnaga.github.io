---
layout: post
title:  "LPF (Low Pass Filter) in C++"
date:   2019-1-21 23:26:00
categories: technology, c++
---

I was trying to pursue kalman filtering methods for one of the self research project, so i never heard of the filtering.

So my initial journey started with the low pass filter.

Low pass filter is used to smoothen the input signal. This, can be used for other purposes such as noise removal etc.

A simple low pass filter equation is :

```matlab

y(n) = alpha * x(n) + (1 - alpha) * y(n-1);

```


the y define the set of output signals while x define the input. The y(n) define the current value of the signal at this moment, while y(n-1) define the previous values.


the `alpha` is computed with trial-and-error method, usually with tools and validation. A proper value of alpha provide good smoothening. remeber, that the alpha varies for different algorithm requirements that use the filter output and different inputs.

usually the filter can be operated in frequency domain and thus can be used in a loop where the signal occur in a periodic interval.


for example,

```c

timer_function() {
    
    float lpf_out_prev = 0.0;
    float lpf_output = 0.0;
    
    int sigal_res = signal_receive(signal_data);
    
    lpf_output = lpf(signal_data, lpf_out_prev);
    lpf_out_prev = lpf_out;
    
}


```

where `lpf_output` represents the current output and `lpf_out_prev` represents the previous value of the output.
Observe that the previous value is set to the current value once the filter function returns.


below is the filter code,

```c

float lpf(double signal_data, double prev_data)
{
    double alpha = 0.0125; // compute this in trial-and-error
    
    double filter_res;
    
    filter_res = alpha * signal_data + (1 - alpha) * (prev_data);
    
    return filter_res;
}

```
 



