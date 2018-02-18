attribute float movements;
uniform float time;

void main(){
   vec4 pos = ftransform();

   if(movements == 1.0){
      pos.x = pos.x + sin(time + pos.y * 2.0) / 52.0;
   }
   if(movements == 0.5){
      pos.y = pos.y + sin(time + pos.y * 2.0) / 22.0;
   }

   gl_TexCoord[0] = gl_MultiTexCoord0;
   gl_Position = pos;
}
