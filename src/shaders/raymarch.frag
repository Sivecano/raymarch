#version 460 core

precision highp float;

out vec4 FragColor;

in vec3 pos;

uniform float scale;
uniform vec2 offset;
uniform vec2 center;
uniform vec3 cposition;
uniform vec2 rot;

vec3 cen = vec3(-offset.x,-offset.y, 10);

vec3 modulus (vec3 vector , float cross)
{
	while (abs(vector.x) > cross/2) vector.x -= sign(vector.x)*cross;
	while (abs(vector.y) > cross/2) vector.y -= sign(vector.y)*cross;
	while (abs(vector.z) > cross/2) vector.z -= sign(vector.z)*cross;
	return vector;
} 

vec3 stepmod (vec3 vector , float cross)
{
	if (abs(vector.x) > cross/2) vector.x -= sign(vector.x)*cross;
	if (abs(vector.y) > cross/2) vector.y -= sign(vector.y)*cross;
	if (abs(vector.z) > cross/2) vector.z -= sign(vector.z)*cross;
	return vector;
}

float QqhToRgb(float q1, float q2, float hue)
{
	if (hue > 360) hue -= 360;
	else if (hue < 0) hue += 360;

	if (hue < 60) return q1 + (q2 - q1) * hue / 60;
	if (hue < 180) return q2;
	if (hue < 240) return q1 + (q2 - q1) * (240 - hue) / 60;
	return q1;
}

vec3 HlsToRgb(float h, float l, float s)
{
	float p2;
	if (l <= 0.5) p2 = l * (1 + s);
	else p2 = l + s - l * s;

	float p1 = 2 * l - p2;
	float float_r, float_g, float_b;
	if (s == 0)
	{
		float_r = l;
		float_g = l;
		float_b = l;
	}
	else
	{
		float_r = QqhToRgb(p1, p2, h + 120);
		float_g = QqhToRgb(p1, p2, h);
		float_b = QqhToRgb(p1, p2, h - 120);
	}

	// Convert RGB to the 0 to 255 range.

	return vec3(float_r, float_g, float_b);
}

float cubeSDF(vec3 from, vec3 center, float sidelength)
{
	from -= center;
	from = abs(from);
	return max(from.x, max(from.y, from.z)) - sidelength/2;
}

float sphereSDF(vec3 from, vec3 center, float radius)
{
	from -= center;

	return length(from) - radius;
}

float planeSDF(vec3 from, vec3 point, vec3 normal)
{
	return dot(from - point, normal);
}

float TetrahedronSDF(vec3 from, vec3 center, float sidelength)
{
	for (int i = 0; i < 1; i++)
	{
		from = center + stepmod(from - center, 4);
	}
	
	float f = 0.57735;
	float a = planeSDF(from, vec3(sidelength, sidelength, sidelength) + center, vec3(-f, f, f));
	float b = planeSDF(from, vec3(sidelength, -sidelength, -sidelength) + center, vec3(f, -f, f));
	float c = planeSDF(from, vec3(-sidelength, sidelength, -sidelength) + center, vec3(f, f, -f));
	float d = planeSDF(from, vec3(-sidelength, -sidelength, sidelength) + center, vec3(-f, -f, -f));

	return max(max(a, b), max(c,d));
}

float distance(vec3 from) {
  float len = 20;
  vec3 center = cen;
  float dist = cubeSDF(from, center, len); 
 

  while (dist < len/2)
  {
    len /= 3;
    vec3 off = from -center;
    off.x = sign(off.x);
    off.y = sign(off.y);
    off.z = sign(off.z);
    center += len * off;
    dist = cubeSDF(from, center, len);
  }
  return dist;



    //return TetrahedronSDF(from, cen, 1);
    //return min( max(cubeSDF(from, cen, 2), sphereSDF(from, cen, 2.5)),1);// planeSDF(from, vec3(0,0,18),vec3(0,0,-1)));
}




void main() {
    float FOV = 5.;
    vec3 tpos = pos;
    vec3 dp = normalize(vec3(pos.xy, scale));
    tpos.z = 0;
    float d = .00001;
    int i = 1;
    int max_i = 1000;
	tpos += cposition;

    while (distance(tpos) > d && i < max_i) 
    {
	//tpos = cen + modulus(tpos -cen, 20);
        tpos += distance(tpos) * dp;
        i++;
    }

    FragColor = vec4((i == max_i) ? 0 :1 / pow(log(i), 0.5), 0, 0, 1);
    //FragColor = vec4(0., 1., 0., 1.);
}
