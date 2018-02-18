uniform sampler2D texture;

void main(){
   vec4 black    = vec4(0.0, 0.0, 0.0, 1.0);
   vec4 texcolor = texture2D(texture, gl_TexCoord[0].st);

   float depth   = gl_FragCoord.z * 100.0f - 87f;
   depth         = clamp(depth, 0.0f, 1.0f);

   gl_FragColor  = mix(texcolor, black, depth);
}
