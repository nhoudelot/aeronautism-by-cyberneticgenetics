uniform float time;

void main(){
   vec4 pos = vec4(0.0, time, 0.0, 0.0);
   gl_TexCoord[0] = gl_MultiTexCoord0 - pos;
   gl_Position = ftransform();
}
