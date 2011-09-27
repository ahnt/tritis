String input="GCAAAAGAAAAADAAAAAHAAABDHAAHBDOAAHBDEEAHBDIIAHBDHAAAJLCCAAJLEEAAJLHAAAENCCAAENGCAACGGAAACGDAAACGDCAACGGEAACGMIAACGHAAAOOCCAAOOBBAAOOGCAAABDBAAABHAADBBOAADBBHAODBBGHODBB";

int t=0;
void setup(){
  size(200,300);
  background(0);
  println(input.length());
}

void draw(){
  int x,y;
  for(y=0;y<6;y++){
    char c=input.charAt(t+y);
    int i=(int)c-'A';
    for(x=0;x<4;x++){
      if(((i>>x)&1)==0)
        fill(color(0,0,0));
      else
        fill(color(255,255,255));
      rect(x*50,y*50,50,50);
    }
  }
  t+=6;
  if(t>=input.length())
    t=0;
  delay(400);
}
