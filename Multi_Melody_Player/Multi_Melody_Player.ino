/* MULTIPLE MELODY PLAYER
 * This sketch doesnt' use the delay function. So you can do other things while playing the melody.
 * Also this sketch allow you to add and play multiple melodies.
 * 
 * Currently we have following melodies
 * SAMAJAVARAGAMANA - Ala Vaikunthapurramuloo
 * MASTER THE BLASTER - Master
 * BUJJI - Jagame Thandhiram
 * ENJOY ENJAAMI
 * 
 * you can add more melodies in the melodies.h file if you want
 * 
 * 
 * For more projects visit https://github.com/jinoycj/ 
 * 
 * Jinoy Johnson - 2021
 */
#include "melodies.h"

const uint8_t buzzer=6;//change the buzzer pin number if you need to.

///Play Music Variables///
int tempo=0;
uint8_t melodies=0; 
uint8_t mel_cnt=0;
uint8_t mel_cnt_st=0; //it has 4 states. searching 0, found 1, running 2, finished 3
int mel_arrSize=0;
int wholenote=0;
int divider = 0, noteDuration = 0;
long noteSrt=0; //used to store the millis when the note start to play
                //diff this from current millis() to get the duration
uint8_t playSeq=0; //play sequence
int mel_arr_i=0; //index of notes
uint8_t currMel=0; //number current playing melody
uint8_t fpSeq=0; //full play sequence
///////

bool play_toggle=true;

void setup() {
  mel_arrSize=(sizeof(mel_arr) / sizeof(mel_arr[0]));
  melodies=countMelodies();
}

void loop() {

/*
  if(play_toggle){ //play the melody only if the play_toggle=true
    if(playSeq==0){
    currMel=1;//random(1,melodies); 
    }
    playMelody();
    if(playSeq==0){
      play_toggle=false; //switching play_toggle to false to stop playing.
                //when the melody reaches its end playSeq will be zero. 
    }
  }
 
*/


 /*If you want to play only one melody, then uncomment the above section and comment all in between FULL PLAY START and FULL PLAY END*/
 
 //FULL PLAY START //
 
  if(play_toggle){
    if(fpSeq==0){
      fpSeq=1;
      currMel=0;
    }else if(fpSeq==1){
      fpSeq=2;
      currMel++;
    }else if(fpSeq==2){
      if(currMel<=melodies){
        playMelody();
        if(playSeq==0){//when the melody reaches its end playSeq will be zero. 
          fpSeq=1; //assigning fpSeq=1 so that it can play the next melody
        }
      }else{
        fpSeq=0;
        play_toggle=false; //switching play_toggle to false to stop playing.
                
      }
    }
  }
  
//FULL PLAY END//
}
void playMelody(){
  if(playSeq==0){
    playSeq=1;
    mel_arr_i=0;
    mel_cnt=0;
    mel_cnt_st=0;
    noteDuration=0;
    noteSrt=0;
  }else if(playSeq==1){
    if((millis()-noteSrt)>=noteDuration){
      noTone(buzzer);
      if(mel_arr_i<mel_arrSize){
        procMelody(currMel);
        mel_arr_i++;
      }else{
        playSeq=2;
      }
    }
  }else{
    playSeq=0;
    noTone(buzzer);
  }

}
void procMelody(uint8_t mel_no){
    if(pgm_read_word_near(mel_arr+mel_arr_i)==MEL_SPTR || mel_arr_i==(mel_arrSize-1)){
      if(mel_cnt_st==0){
        mel_cnt++;
        if(mel_cnt==mel_no){
          mel_cnt_st=1;
        }
      }else if(mel_cnt_st==2){
        mel_cnt_st=3;
        playSeq=2;
      }
    }
    if(mel_cnt_st==1){
      mel_arr_i++; //advancing one step so arr_i will be there at tempo element and from next 
            // arr_i will start the notes
      tempo=pgm_read_word_near(mel_arr+mel_arr_i);
      wholenote= (60000 * 4) / tempo;
      mel_cnt_st=2;
    }else if(mel_cnt_st==2){
      divider = pgm_read_word_near(mel_arr+mel_arr_i + 1);
      if (divider > 0) {
        noteDuration = (wholenote) / divider;
      } else if (divider < 0) {
  
        noteDuration = (wholenote) / abs(divider);
        noteDuration *= 1.5; // increases the duration in half for dotted notes
      }
      noteSrt=millis();
      tone(buzzer, pgm_read_word_near(mel_arr+mel_arr_i), noteDuration * 0.9);
      mel_arr_i++;
   }
}
uint8_t countMelodies(){
  uint8_t mel_cnt=0;
  for(int arr_i=0;arr_i<mel_arrSize;arr_i++){
    if(pgm_read_word_near(mel_arr+arr_i)==MEL_SPTR){
      mel_cnt++;
    }
  }
  return mel_cnt;
}
