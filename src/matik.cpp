// http://www.cse.yorku.ca/~oz/marsaglia-rng.html
#define rng_znew (rng_z=36969*(rng_z&65535)+(rng_z>>16))
#define rng_wnew (rng_w=18000*(rng_w&65535)+(rng_w>>16))
#define MWC ((rng_znew<<16)+rng_wnew )
uint32_t rng_z=362436069, rng_w=521288629;
//uint64_t rng_z=362436069, rng_w=521288629;


vec2 v2(float x, float y)
{
	vec2 new_vec2 = { x, y };
	return new_vec2;
}
vec2 v2_add(vec2 a, vec2 b) { return v2( a.x + b.x, a.y + b.y ); }
vec2 v2_sub(vec2 a, vec2 b) { return v2( a.x - b.x, a.y - b.y); }
float v2_length( vec2 a) { return sqrtf(a.x*a.x + a.y*a.y); }

// what to do when value is higher than INT_MAX?
int ClampTo(int min, int max, int value)
{
	if ( value < min ) { return min; }
	else if ( value > max ) { return max; }
	else { return value; }
}

float ClampToF(float min, float max, float value)
{
	if ( value < min ) { return min; }
	else if ( value > max ) { return max; }
	else { return value; }
}
