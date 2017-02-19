#include<stdio.h>
int main(){
  int array[1000], position, num, index=0, count=0;
  FILE *file;
  file = fopen("TEST.dat", "r");
  if (file) {
    while (fscanf(file, "%d", &num)!=EOF){
      if(index==0)
        index = num;
      else{
        array[count] = num;
        count++;
      }
    }
    fclose(file);
  }
  for(int i=0; i<index; i++){
    position = i;
    for(int j=i+1; j<index; j++){
      if(array[position] > array[j])
        position = j;
    }
    if(position != i){
      int tmp = array[i];
      array[i] = array[position];
      array[position] = tmp;
    }
  }
  for(int i=0; i<index; i++){
    printf("%d\n", array[i]);
  }
}