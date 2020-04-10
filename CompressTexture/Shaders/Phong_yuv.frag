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
	texcoord.x = v_tex_coord.x;
	texcoord.y = 1 -  v_tex_coord.y;

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


	
	if(u_flag_texture_diffrence == false){	
		final_color = vec4(r,g,b,1.0);
	}
	else{
	
		vec4 original_color, tex_color;
		original_color = texture(u_original_texture, v_tex_coord);
		vec2 tex_coord = vec2(v_tex_coord.x, v_tex_coord.y);

		if(u_flag_texture_reverse == true){
			tex_coord = vec2(v_tex_coord.x, 1-v_tex_coord.y);
		}

		tex_color = vec4(r,g,b,1.0);

	
		final_color.x = 1.0 - abs(original_color.x - tex_color.x);
		final_color.y = 1.0 - abs(original_color.y - tex_color.y);
		final_color.z = 1.0 - abs(original_color.z - tex_color.z);
	}
}
