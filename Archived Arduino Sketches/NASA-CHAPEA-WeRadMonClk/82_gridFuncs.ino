
void printText(char* s, int startCol){
  int column = startCol;
  while (*s != 0){
    printChar(*s,column);
    s++;
    column += buffer[0]+1;
  }
}

void printChar(char c,int column){
  if (c<32) return;
  c-=32;
  memcpy_P(buffer, CH + 7 * c, 7);
  grid.writeSprite(column, 0, buffer);
  grid.setColumn(column + buffer[0], 0);

}
