uniform sampler2D texture;
uniform float percentage;

void main(){
  float xpos = max(gl_TexCoord[0].s, percentage);
  vec2 pos   = vec2(xpos, gl_TexCoord[0].t);
  vec4 texel = texture2D(texture, pos);

  gl_FragColor = texel;
}
