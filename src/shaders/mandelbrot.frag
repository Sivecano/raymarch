#version 460 core

precision highp float;

out vec4 FragColor;

in vec3 pos;

uniform float scale;
uniform vec2 offset;
uniform vec2 center;

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

void main() {
    
    float x0 = (pos.x / scale + 1)* 1.75 + offset.x - 2.5;
    float y0 = pos.y / scale + offset.y;
    float x;
    float y;
    const int max_i = 1000;
    int i = 0;
    float tempx;

    while(x*x + y*y < 3 && i < max_i)
    {
        tempx = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = tempx;
        i++;
    }

    FragColor = vec4(HlsToRgb(270 *i /max_i, (i == max_i)? 0 :.5, .5), 1);
    //FragColor = vec4(0., 1., 0., 1.);
}
