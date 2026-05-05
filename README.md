# Waveform-Generator-PSoC-
This project generates different waveforms using a PSoC microcontroller and VDAC output. The waveform can be controlled in real-time using ADC inputs.

**Features**
1. Generate:
  Sine wave (approximation)
  Square wave
  Triangle wave
2. Control frequency using ADC
3. Control amplitude using ADC
4. Switch between waves using a push button
**How It Works**
1. Lookup tables are used to store waveform values
2. Timer + Interrupt is used to output samples
3. ADC reads:
  Frequency input
  Amplitude input
4. Output is sent through VDAC
**Hardware Used**
 PSoC
 VDAC8
 ADC (SAR)
 Analog Mux
  Timer
  Push Button
**Usage**
Build and upload the code to the PSoC
Use potentiometers to control:
Frequency
Amplitude
Press the button to change waveform
**Notes**
- Sine wave is approximated (not exact)
- Frequency depends on timer period
- Amplitude is scaled digitally
Timer
Push Button
