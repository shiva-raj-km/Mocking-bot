## Mocking Bot - Task4 : Instrument Classification

#  Instructions
#  ------------
#
#  This file contains Main function and Instrument_identify function. Main Function helps you to check your output
#  for practice audio files provided. Do not make any changes in the Main Function.
#  You have to complete only the Instrument_identify function. You can add helper functions but make sure
#  that these functions are called from Instrument_identify function. The final output should be returned
#  from the Instrument_identify function.
#
#  Note: While evaluation we will use only the onset_detect function. Hence the format of input, output
#  or returned arguments should be as per the given format.
#  
#  Recommended Python version is 2.7.
#  The submitted Python file must be 2.7 compatible as the evaluation will be done on Python 2.7.
#  
#  Warning: The error due to compatibility will not be entertained.
#  -------------


'''

* Author List:      SHIVARAJ K M
* Filename:         Intrument_classifier.py
* Theme:            MOCKING BOT
* Functions:        Instrument_identify , detect_note_duration , detect_notes , load_network , serial_communication , main 
* Global Variables: window_size , sampling_freq , threshold , frequency_array , notes
'''

## Library initialisation

# Import Modules
# DO NOT import any library/module
# related to Audio Processing here
import numpy as np
import math
import wave
import os
import struct
from pydub import AudioSegment
from scipy.io import wavfile
from essentia.standard import *
import serial                    # For serial communication
import time                      

window_size = 512      # Size of window to be used for detecting silence
sampling_freq = 44100  # Sampling frequency of audio signal
threshold = 0.001      # Threshold value to detect the silence between the notes


frequency_Array = [16.35,  17.42,  18.35,  19.45,  20.60,   21.83,  23.12,  24.50,   25.96,   27.50,   29.14,  30.87,
         32.70,  34.65,  36.71,  38.89,  41.20,   43.65,  46.25,   49.00,  51.91,   55.00,   58.27,   61.74,
         65.41,  69.30,  73.42,  77.78,  82.41,   87.31,  92.50,   98.00,  103.83,  110.00,  116.54,  123.47,
         130.81, 138.59, 146.83, 146.83, 164.81,  174.61, 185.00,  196.00, 207.65,  220.00,  233.08 , 246.94,
         261.63, 277.18, 293.66, 311.13, 329.63,  349.23, 369.99,  392.00, 415.30,  440.00,  466.16,  493.88,
         523.25, 554.37, 587.33, 622.25, 659.25,  698.46, 739.99,  783.99, 830.61,  880.00,  923.77,  987.77,
         1046.50,1108.73,1174.66,1244.51,1318.51, 1396.91,1479.98, 1567.98,1661.22, 1760.00, 1864.66, 1975.53,
         2093.00,2217.46,2349.32,2489.32,2637.02, 2793.83,2959.96, 3135.96,3322.44, 3520.00, 3729.31, 3951.07,
         4186.01,4434.92,4698.63,4978.03,5274.04, 5587.65,5919.91, 6271.93,6644.88, 7040.00, 7458.62, 7902.13 ]

notes = ['C0',    'C#0', 'D0',   'D#0',  'E0',    'F0',   'F#0',   'G0',   'G#0',    'A0',   'A#0',   'B0',
         'C1',    'C#1', 'D1',   'D#1',  'E1',    'F1',   'F#1',   'G1',   'G#1',    'A1',   'A#1',   'B1',
         'C2',    'C#2', 'D2',   'D#2',  'E2',    'F2',   'F#2',   'G2',   'G#2',    'A2',   'A#2',   'B2',
         'C3',    'C#3', 'D3',   'D#3',  'E3',    'F3',   'F#3',   'G3',   'G#3',    'A3',   'A#3',   'B3',
		 'C4',    'C#4', 'D4',   'D#4',  'E4',    'F4',   'F#4',   'G4',   'G#4',    'A4',   'A#4',   'B4',
         'C5',    'C#5', 'D5',   'D#5',  'E5',    'F5',   'F#5',   'G5',   'G#5',    'A5',   'A#5',   'B5',
		 'C6',    'C#6', 'D6',   'D#6',  'E6',    'F6',   'F#6',   'G6',   'G#6',    'A6',   'A#6',   'B6',
         'C7',    'C#7', 'D7',   'D#7',  'E7',    'F7',   'F#7',   'G7',   'G#7',    'A7',   'A#7',   'B7',
         'C8',    'C#8', 'D8',   'D#8',  'E8',    'F8',   'F#8',   'G8',   'G#8',    'A8',   'A#8',   'B8']



# Teams can add helper functions
# Add all helper functions here

def load_network(segment_audio_file):

	instrument_pred = ""
	instrument = []
	sr, data = wavfile.read(segment_audio_file)

	if data.dtype != np.int16:
	    raise TypeError('Bad sample type: %r' % data.dtype)


	from audio.processor import WavProcessor, format_predictions

	with WavProcessor() as processor:
	    predictions = processor.get_predictions(sr, data)

	instrument[:] = format_predictions(predictions)

	for i in range(len(instrument[:])):
		if (instrument[i] != ':'):
			instrument_pred += (str(instrument[i]))
		else:
			break
	return ''.join(instrument_pred)


'''
* Function Name: detect_note_duration
* Input:         Audio file data containing of multiple notes in .wav format
* Output:        Note durations of each note present in the audio file from this onset can be extracted
* Logic:         Based on silence note duration is detected 
* Example Call:  detect_note_duration(audio_file)
'''


def detect_note_duration(audio_file):
	
	Note_durations = []
	file_length = audio_file.getnframes()     # Unpacking of audio signal using struct library
	sound = np.zeros(file_length)
	sound_square = np.zeros(file_length)
	for i in range(file_length):
	    data = audio_file.readframes(1)
	    data = struct.unpack("<h", data)
	    sound[i] = int(data[0])	
	sound = np.divide(sound, float(2 ** 15))  # Normalize data in range -1 to 1
	sound_square = np.square(sound)


	frequency = []
	dft = []
	c = 0         # Declaration and initialiazation of some variable required for detecting note duration
	i = 0
	j = 0
	k = 0
	start = 0     # To store the start time of note
	end = 0       # To store the end time of note
	diff = 0
	prev_end = 0
	x = []
	m = 0
	# traversing sound_square array with a fixed window_size
	while (i <= len(sound_square) - window_size):
	    s = 0.00
	    j = 0
	    while (j <= window_size):
	        s = s + sound_square[i + j]
	        j = j + 1
	    if s > threshold:
	        c = c+1
	        start = i
	        end = i + window_size - 1
	        diff = start - prev_end

	        if (diff != 1 ):	           
	           	x.append(prev_end)
	        	x.append(start)
	        prev_end = end
	    k = i + 1
	    i = i + window_size
	x.append(end)
	silence = []
	for i in range(len(x)):
		silence.append(round((x[i] / 44100.0),2))
	note_duration = []
	i = 0
	silence.pop(0)
	for b in range(len(silence)/2):
		if(abs(silence[i] - silence[i+1]) > 0.05):
			a = [silence[i],silence[i+1]]
			Note_durations.append(a)
		i= i+2
	audio_file.rewind()

	return Note_durations


'''
* Function Name: detect_notes
* Input:         Audio file name
* Output:        notes present in the audio file  
* Logic:         pitch detection algorithm of essentia is used to get the pitch frequency in Hz and then mapping 
*                is done by using frequency array and notes array  
* Example Call:  detect_notes(audio_file_name)
'''



def detect_notes(file_name):

	audio = MonoLoader(filename=file_name)()           
	w = Windowing(type = 'hann')
	spectrum = Spectrum()  
	pitch = PitchYinFFT()
	final = []
	pitch_hz = []

	for frame in FrameGenerator(audio, frameSize = 2048, hopSize = 1024):
		spec = spectrum(w(frame))
		pitch_freq,confidence = pitch(spec)
		if(confidence >= 0.8):
			pitch_hz.append(pitch_freq)
	final.append(round(pitch_hz[0],2))
	a = 0
	for i in range(len(pitch_hz)):
		if(abs(pitch_hz[i]-final[a])>20):
			final.append(float(round(pitch_hz[i],2)))
			a = a + 1
	Detected_Note = []
	Identified_Notes = []
	for i in final:		   # To determine note of the coressponding frequency
		idx = (abs(np.subtract(frequency_Array , i))).argmin()
		Identified_Notes.append(notes[idx])
	Detected_Note = Identified_Notes

	return Detected_Note

############################### Your Code Here #############################################

'''
* Function Name: Instrument_identify
* Input:         Audio file data and file name
* Output:        All detected intruments, notes and onsets array  
* Logic:         In this all the above functions are called the given audio file is segmented using 
                 pydub library and then passed to the load network to get the predictions of instrument  
* Example Call:  Instrument_identify(audio_file_data,audio_file_name)
'''

############################### Your Code Here #############################################

def Instrument_identify(audio_file,file_name):
	
	#   Instructions
	#   ------------
	#   Input 	:	audio_file -- a single test audio_file as input argument
	#   Output	:	1. Instruments -- List of string corresponding to the Instrument
	#			2. Detected_Notes -- List of string corresponding to the Detected Notes
        #                       3. Onsets -- List of Float numbers corresponding
	#			        to the Note Onsets (up to Two decimal places)
	#   Example	:	For Audio_1.wav file,
	# 				Instruments = ["Piano","Violin","Piano","Trumpet"]
	#				Detected_Notes = ["C3","B5","A6","C5"]
	#                               Onsets = [0.00, 0.99, 1.32, 2.04]
        
	# Add your code here
	Instruments = []
	Detected_Notes = []
	Onsets = []
	Detected_Notes1 = []

	note_durations = detect_note_duration(audio_file)
	# print(note_durations)
	a = 0
	for i in range(len(note_durations)):
		t1 =  note_durations[a]
		c = t1[0] * 1000
		d = t1[1] * 1000               #Works in milliseconds
		Onsets.append(c/1000)
		newAudio = AudioSegment.from_wav(file_name)
		newAudio = newAudio[c:d]
		if(abs(c-d)<1000):
			silence = AudioSegment.silent(duration=1000) 
			newAudio = silence.overlay(newAudio)
		newAudio.export('segment.wav', format="wav") 
		a = a + 1
		segment_file_name = path + "/segment.wav"
		instrument_type = load_network(segment_file_name)
		Detected_Notes1 = detect_notes(segment_file_name)
		# print(Detected_Notes1)
		if(len(Detected_Notes1) > 1):
			Detected_Notes.append(Detected_Notes1[1])
		else:
			Detected_Notes.append(Detected_Notes1[0])
		Instruments.append(str(instrument_type))


	return Instruments, Detected_Notes, Onsets



'''
* Function Name: serial_communication
* Input:         Instruments detected, Detected_notes, Onsets_detected using machine learning
* Output:        The extracted data is transmited to the atmega board of Bot part at a baud rate of 9600 bps.
* Logic:         The output of machine learning is processed and new array is used to transmit the information to the bot part of atmega board. 
* Example Call:  serial_communication(Instruments,Detected_Notes,Onsets)
'''

'''def serial_communication(Instruments, Detected_Notes, Onsets):
	serial_instruments = []
	serial_onsets = []
	serial_notes = []
	note = []
	for i in range(len(Instruments)):                 # array processing for serial communication 
		note = Detected_Notes[i]
		if(Instruments[i] == 'Piano'):
			serial_instruments.append('P')
			serial_notes.append(note[0])
			serial_onsets.append(Onsets[i])
		if(Instruments[i] == 'Trumpet'):
			serial_instruments.append('T')
			serial_notes.append(note[0])
			serial_onsets.append(Onsets[i])
	# print serial_instruments,serial_notes,serial_onsets
	ser = serial.Serial("/dev/ttyUSB0", 9600, timeout=0.005)


	total=0
	for word in serial_instruments:
		total += len(word)
	final = total + (len(serial_instruments)) 
	print final
	# total=0
	# for word in onsets:
	# 	total += len(word)
	# final = total + (len(onsets)) 
	# print final
	total=0
	for word in serial_notes:
		total += len(word)
	final = total + (len(serial_notes)) 
	print final

	for i in range(len(serial_instruments)):             # Transmiting instrument array to serial port
		if ser.isOpen():
				ser.flushInput()
				ser.flushOutput()
				ser.write(str.encode(str(serial_instruments[i])))
				ser.write(str.encode(str(',')))

				time.sleep(0.1)
				numOfLines=0

				while True:                              # receiving the transmited data from atmega board
					response=ser.readline()
					print(response)

					break
					ser.close()

	for i in range(len(serial_onsets)):                  # Transmiting onsets to serial port
		if ser.isOpen():
			ser.flushInput()
			ser.flushOutput()
			ser.write(str.encode(str(serial_onsets[i])))
			ser.write(str.encode(str(',')))

			time.sleep(0.1)
			numOfLines=0

			while True:                                 # receiving the transmited data from atmega board
				response=ser.readline()
				print(response)

				break
				ser.close()


	for i in range(len(serial_notes)):                   # Transmiting notes to serial port    
		if ser.isOpen():
			ser.flushInput()
			ser.flushOutput()

			ser.write(str.encode(str(serial_notes[i])))
			ser.write(str.encode(str(',')))

			time.sleep(0.1)
			numOfLines=0

			while True:                                # receiving the transmited data from atmega board
				response=ser.readline()
				print(response)

				break
				ser.close()     

	while 1:
		response=ser.readline()
		print(response)


'''

############################### Main Function #############################################

if __name__ == "__main__":

	#   Instructions
	#   ------------
	#   Do not edit this function.

	# code for checking output for single audio file
	path = os.getcwd()
	
	file_name = path + "/Audio_files/Original_audio.wav"
	audio_file = wave.open(file_name)
	
	Instruments, Detected_Notes, Onsets = Instrument_identify(audio_file,file_name)

	print("\n\tInstruments = "  + str(Instruments))
	print("\n\tDetected Notes = " + str(Detected_Notes))
	print("\n\tOnsets = " + str(Onsets))

	serial_communication(Instruments,Detected_Notes,Onsets)

