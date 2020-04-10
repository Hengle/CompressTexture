#version 420

uniform sampler2D u_original_texture;

uniform sampler2D u_texture_y;
uniform sampler2D u_texture_u;
uniform sampler2D u_texture_v;

uniform int textureIndex;	//1~3. 1 texture has 3 image.

uniform bool u_flag_texture_reverse = false;
uniform bool u_flag_texture_mapping = true;
uniform bool u_flag_texture_diffrence = false;

in vec3 v_position_EC;
in vec3 v_normal_EC;
in vec2 v_tex_coord;

layout (location = 0) out vec4 final_color;


void main(void) {

	vec4 base_color, shaded_color;

	vec2 texcoord;
	texcoord.x =  1.0 - v_tex_coord.x;
	texcoord.y =  v_tex_coord.y;

	float y,u,v;
	if(textureIndex == 1){	
		y = texture2D(u_texture_y, texcoord).b;  
		u = texture2D(u_texture_u, texcoord).b - 0.5;  
		v = texture2D(u_texture_v, texcoord).b - 0.5;  
	}
	else if(textureIndex == 2){	
		y = texture2D(u_texture_y, texcoord).g;  
		u = texture2D(u_texture_u, texcoord).g - 0.5;  
		v = texture2D(u_texture_v, texcoord).g - 0.5;  
	}
	else{	
		y = texture2D(u_texture_y, texcoord).r;  
		u = texture2D(u_texture_u, texcoord).r - 0.5;  
		v = texture2D(u_texture_v, texcoord).r - 0.5;  
	}

    float r = y +             1.402 * v;  
    float g = y - 0.344 * u - 0.714 * v;  
    float b = y + 1.772 * u;  

    final_color = vec4(r,g,b,1.0);

}
