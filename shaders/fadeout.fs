uniform sampler2D texture;
uniform float percent;

void main(){
   vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
   vec4 color = texture2D(texture, gl_TexCoord[0].st);
   gl_FragColor = mix(color, black, percent);
}
