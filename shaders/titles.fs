uniform sampler2D who;
uniform float aspect;

void main(){

  /* Set the texture position in the screen */
  float height = 1.0 / aspect;
  vec2 pos = vec2(gl_TexCoord[0].s, gl_TexCoord[0].t / height - (1.0 - height / 2.0));
  pos = clamp(pos, 0.0, 1.0);
  vec4 color = texture2D(who, pos);

  gl_FragColor = color;
}
