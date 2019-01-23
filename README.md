# Audio-Analysis

A C++ Application that takes audio input in the form a mp3 or wav file and processes it, separating audio sources and converting it into lead sheets.



From the wav file a series of vectors will be extracted.
Each vector will be length 128 organized as below.
  There are ten full octaves plus 8/12ths of an octave and twelve notes per octave (except the last, which only has eight). 
  (The labeling used below has the note name preceding the underscore and the octave number following the underscore, know that each element of the vector will be a real valued number indicating the volume of that note).

{C_0, C#_0, D_0, D#_0, E_0, F_0, F#_0, G_0, G#_0, A_0, A_0, B_0, C_1, C#_1, D_1, D#_1, E_1, F_1, F#_1, G_1, G#_1, A_1, A_1, B_1, C_2, C#_2, D_2, D#_2, E_2, F_2, F#_2, G_2, G#_2, A_2, A_2, B_2, C_3, C#_3, D_3, D#_3, E_3, F_3, F#_3, G_3, G#_3, A_3, A_3, B_3, C_4, C#_4, D_4, D#_4, E_4, F_4, F#_4, G_4, G#_4, A_4, A_4, B_4, C_5, C#_5, D_5, D#_5, E_5, F_5, F#_5, G_5, G#_5, A_5, A_5, B_5, C_6, C#_6, D_6, D#_6, E_6, F_6, F#_6, G_6, G#_6, A_6, A_6, B_6, C_7, C#_7, D_7, D#_7, E_7, F_7, F#_7, G_7, G#_7, A_7, A_7, B_7, C_8, C#_8, D_8, D#_8, E_8, F_8, F#_8, G_8, G#_8, A_8, A_8, B_8, C_9, C#_9, D_9, D#_9, E_9, F_9, F#_9, G_9, G#_9, A_9, A_9, B_9, C_10, C#_10, D_10, D#_10, E_10, F_10, F#_10, G_10}

  #### I have formated the above vector by 12 so the pattern is more clear.
  {   C_0,		C#_0,		D_0,		D#_0,		E_0,		F_0,		F#_0,		G_0,		G#_0,		A_0,		A_0,		B_0,		<br/>
      C_1,		C#_1,		D_1,		D#_1,		E_1,		F_1,		F#_1,		G_1,		G#_1,		A_1,		A_1,		B_1,		<br/>
      C_2,		C#_2,		D_2,		D#_2,		E_2,		F_2,		F#_2,		G_2,		G#_2,		A_2,		A_2,		B_2,		<br/>
      C_3,		C#_3,		D_3,		D#_3,		E_3,		F_3,		F#_3,		G_3,		G#_3,		A_3,		A_3,		B_3,		<br/>
      C_4,		C#_4,		D_4,		D#_4,		E_4,		F_4,		F#_4,		G_4,		G#_4,		A_4,		A_4,		B_4,		<br/>
      C_5,		C#_5,		D_5,		D#_5,		E_5,		F_5,		F#_5,		G_5,		G#_5,		A_5,		A_5,		B_5,		<br/>
      C_6,		C#_6,		D_6,		D#_6,		E_6,		F_6,		F#_6,		G_6,		G#_6,		A_6,		A_6,		B_6,		<br/>
      C_7,		C#_7,		D_7,		D#_7,		E_7,		F_7,		F#_7,		G_7,		G#_7,		A_7,		A_7,		B_7,		<br/>
      C_8,		C#_8,		D_8,		D#_8,		E_8,		F_8,		F#_8,		G_8,		G#_8,		A_8,		A_8,		B_8,		<br/>
      C_9,		C#_9,		D_9,		D#_9,		E_9,		F_9,		F#_9,		G_9,		G#_9,		A_9,		A_9,		B_9,		<br/>
      C_10,		C#_10,	D_10,		D#_10,	E_10,		F_10,		F#_10,	G_10   }
