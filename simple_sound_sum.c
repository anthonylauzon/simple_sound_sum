// author: ant@systematic.systems

#include <math.h>
#include <stdlib.h>
#include <float.h>
#include <sndfile.h>

#define MIN_BUFFER_LEN 158760000

int main(int argc, char **argv) {
  int num_sounds = argc-2;

  if (num_sounds < 2)
    return 1;

  const char *outfile = argv[argc-1];

  SNDFILE **sounds = (SNDFILE **) malloc(num_sounds * sizeof(SNDFILE *));
  SF_INFO *sound_info = (SF_INFO *) malloc(num_sounds * sizeof(SF_INFO));
  float **sound_buffers = (float **) malloc(num_sounds * sizeof(float *));
  
  int i;
  int min_buffer_len = MIN_BUFFER_LEN;

  for (i = 0; i < num_sounds; i++) {
    sound_info[i].format = 0;
    sounds[i] = sf_open(argv[i+1], SFM_READ, &sound_info[i]);
    int buffer_len = sound_info[i].frames * sound_info[i].channels;
    sound_buffers[i] = (float *) malloc(buffer_len * sizeof(float));

    sf_read_float(sounds[i], sound_buffers[i], buffer_len);
    
    if (buffer_len < min_buffer_len) {
      min_buffer_len = buffer_len;
    }
  }
    
  float *sumsound_buffer = (float *) malloc(min_buffer_len * sizeof(float));
  int cur_sample;

  for (cur_sample = 0; cur_sample < min_buffer_len; cur_sample++) {
    sumsound_buffer[cur_sample] = 0.0;

    for (i = 0; i < num_sounds; i++) {
      sumsound_buffer[cur_sample] = sumsound_buffer[cur_sample] + 
      sound_buffers[i][cur_sample];
    }
  }

  SNDFILE *sumsound = sf_open(outfile, SFM_WRITE, &sound_info[0]);
  sf_write_float(sumsound, sumsound_buffer, min_buffer_len);

  for (i = 0; i < num_sounds; i++) {
    free(sound_buffers[i]);
    sf_close(sounds[i]);
  }
  
  free(sound_buffers);
  free(sound_info);
  free(sounds);
  
  free(sumsound_buffer);
  sf_close(sumsound);

  return 0;
}
