#version 420

uniform sampler2D u_base_texture;
uniform sampler2D u_original_texture;

uniform bool u_flag_texture_reverse = false;
uniform bool u_flag_texture_mapping = true;
uniform bool u_flag_texture_diffrence = false;

in vec3 v_position_EC;
in vec3 v_normal_EC;
in vec2 v_tex_coord;

layout (location = 0) out vec4 final_color;


void main(void) {
	vec4 base_color, shaded_color;

	if(u_flag_texture_diffrence == false){
		if (u_flag_texture_mapping) {
			if(u_flag_texture_reverse == true){
				vec2 tex_coord = vec2(v_tex_coord.x, 1-v_tex_coord.y);
				base_color = texture(u_base_texture, tex_coord);
			}
			else
				base_color = texture(u_base_texture, v_tex_coord);

		}
		else 
			base_color = vec4(1,1,0,1);

		final_color = base_color;
	}
	else{
	
		vec4 original_color, tex_color;
		original_color = texture(u_original_texture, v_tex_coord);
		vec2 tex_coord = vec2(v_tex_coord.x, v_tex_coord.y);

		if(u_flag_texture_reverse == true){
			tex_coord = vec2(v_tex_coord.x, 1-v_tex_coord.y);
		}

		tex_color = texture(u_base_texture, tex_coord);

	
		final_color.x = 1.0 - abs(original_color.x - tex_color.x);
		final_color.y = 1.0 - abs(original_color.y - tex_color.y);
		final_color.z = 1.0 - abs(original_color.z - tex_color.z);
	}
}
