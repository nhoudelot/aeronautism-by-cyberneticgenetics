uniform sampler2D balloon;
uniform sampler2D tilling;
uniform sampler2D newimage;
uniform float zoom;

void main(){
   vec2 tc; // texture coordinate
   vec3 texcolor;

   tc = 1.0 - gl_TexCoord[0].st * (1.0 - 2.0 * zoom) - zoom;
   texcolor = texture2D(balloon, tc).rgb;

   /* If we zoom in enough break the image into smaller pices */
   if(zoom < 0.6){
      float pices      = (1.0 / 389.0);
      float percentage = (zoom - 0.6) * -10.2; //percentage of the color mixture
      vec2 tc_tilling  = tc / pices;
      vec2 pos         = fract(tc_tilling) / 3.0;

      /* Choose subimage in the tilling texture */
      float compoundcolor = texcolor.r * 100.0 + texcolor.g * 10.0 + texcolor.b;
      float tillnum       = floor(compoundcolor / 12.333);
      float offsety       = floor(tillnum / 3.0);
      float offsetx       = floor(tillnum - offsety * 3.0);
      pos = pos + (1.0 / 3.0) * vec2(offsetx, offsety);

      /* We change the tilling image in the center */
      vec3 tillingcolor; 
      if(tc.x > 0.498 && tc.y > 0.498 && tc.x < 0.502 && tc.y < 0.502)
         tillingcolor = texture2D(newimage, pos * 3.0).rgb;
      else
         tillingcolor = texture2D(tilling, pos).rgb;

      /* Compound the original texture color and the tilling texture color to hide the differences */
      texcolor         = mix(texcolor, tillingcolor, percentage);
   }

   gl_FragColor = vec4(texcolor, 1.0);
}
