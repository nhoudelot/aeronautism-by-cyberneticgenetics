uniform sampler2D texture;

void main(){
   vec4 color = texture2D(texture, gl_TexCoord[0].st);
   color = min(color + 0.95, 1.0);
   gl_FragColor = color;
}
