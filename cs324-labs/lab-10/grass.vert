#extension GL_EXT_gpu_shader4: enable

uniform int v_ticks;

int LFSR_Rand_Gen(in int n)
{
  // <<, ^ and & require GL_EXT_gpu_shader4.
  n = (n << 13) ^ n; 
  return (n * (n*n*15731+789221) + 1376312589) & 0x7fffffff;
}

float LFSR_Rand_Gen_f( in int n )
{
  return float(LFSR_Rand_Gen(n));
}

float my_noise3(in vec3 p)
{
  ivec3 ip = ivec3(floor(p));
  vec3 u = fract(p);
  u = u*u*(3.0-2.0*u);

  int n = ip.x + ip.y*57 + ip.z*113;

  float res = mix(mix(mix(LFSR_Rand_Gen_f(n+(0+57*0+113*0)),
                          LFSR_Rand_Gen_f(n+(1+57*0+113*0)),u.x),
                      mix(LFSR_Rand_Gen_f(n+(0+57*1+113*0)),
                          LFSR_Rand_Gen_f(n+(1+57*1+113*0)),u.x),u.y),
                 mix(mix(LFSR_Rand_Gen_f(n+(0+57*0+113*1)),
                          LFSR_Rand_Gen_f(n+(1+57*0+113*1)),u.x),
                      mix(LFSR_Rand_Gen_f(n+(0+57*1+113*1)),
                          LFSR_Rand_Gen_f(n+(1+57*1+113*1)),u.x),u.y),u.z);

  return 1.0 - res*(1.0/1073741824.0);
}


void main(void)
{
	gl_Position =  gl_Vertex;
	gl_FrontColor = gl_Color;

	// comment out the following if noise3() return zero
	//vec3 rand = noise3(gl_Vertex.xyz); // when noise3() works

	// comment in the following if noise3() returns zero
	vec3 rand = vec3(my_noise3(gl_Vertex.xyz), 0.0, my_noise3(gl_Vertex.zyx));

	float phase = 2.0*atan(1.0)*4.0*(float(v_ticks)/180.0); // radians

	gl_FrontSecondaryColor = vec4(rand, phase);
}
