#version 420

//0-original image, 1-comopressed image
uniform int u_drawtype;

//0 - rgb image, 1 - yuv image
uniform int u_type_of_original_image;
uniform int u_type_of_compressed_image;

uniform sampler2D u_original_texture;
uniform sampler2D u_original_texture_y;
uniform sampler2D u_original_texture_u;
uniform sampler2D u_original_texture_v;
uniform int u_original_textureIndex;	//1~3. 1 yuv texture has 3 image.

uniform sampler2D u_base_texture;
uniform sampler2D u_base_texture_y;
uniform sampler2D u_base_texture_u;
uniform sampler2D u_base_texture_v;
uniform int u_base_textureIndex;	//1~3. 1 yuv texture has 3 image.

uniform bool u_flag_texture_reverse = false;
uniform bool u_flag_texture_mapping = true;
uniform bool u_flag_texture_diffrence = false;

in vec3 v_position_EC;
in vec3 v_normal_EC;
in vec2 v_tex_coord;

layout (location = 0) out vec4 final_color;


void main(void) {

	vec4 base_color;
	vec4 original_color = texture(u_base_texture_y, v_tex_coord);
	vec2 coord = vec2(v_tex_coord.x, 1 - v_tex_coord.y);
	base_color = texture(u_base_texture_u, coord);
	final_color.x = 1.0 - abs(original_color.r - base_color.g)*100;
	final_color.y = 1;
	final_color.z = 1;
	//final_color = original_color;

	return;

	//original image
	if (u_drawtype == 0) {
		vec2 tex_coord2;
		tex_coord2 = vec2(v_tex_coord.x, 1 - v_tex_coord.y);
		if (u_type_of_original_image == 0) {//rgb image
			final_color = texture(u_original_texture, tex_coord2);
		}
		else if (u_type_of_original_image == 1) {//yuv image

			float y, u, v;
			if (u_original_textureIndex == 1) {
				y = texture2D(u_original_texture_y, tex_coord2).b;
				u = texture2D(u_original_texture_u, tex_coord2).b - 0.5;
				v = texture2D(u_original_texture_v, tex_coord2).b - 0.5;
			}
			else if (u_original_textureIndex == 2) {
				y = texture2D(u_original_texture_y, tex_coord2).g;
				u = texture2D(u_original_texture_u, tex_coord2).g - 0.5;
				v = texture2D(u_original_texture_v, tex_coord2).g - 0.5;
			}
			else {
				y = texture2D(u_original_texture_y, tex_coord2).r;
				u = texture2D(u_original_texture_u, tex_coord2).r - 0.5;
				v = texture2D(u_original_texture_v, tex_coord2).r - 0.5;
			}

			float r = y + 1.402 * v;
			float g = y - 0.344 * u - 0.714 * v;
			float b = y + 1.772 * u;

			final_color = vec4(r, g, b, 1.0);
		}

	}
	else {//comprssed image

		//get texture color

		if (u_type_of_compressed_image == 0) {//rgb image

			vec2 tex_coord;
			if (u_flag_texture_reverse == true) {
				tex_coord = vec2(v_tex_coord.x, 1 - v_tex_coord.y);
			}
			else {
				tex_coord = vec2(v_tex_coord.x, v_tex_coord.y);
			}

			base_color = texture(u_base_texture, tex_coord);
		}
		else if (u_type_of_compressed_image == 1) {//yuv image

			vec2 tex_coord;
			if (u_flag_texture_reverse == true) {
				tex_coord = vec2(v_tex_coord.x, 1 - v_tex_coord.y);
			}
			else {
				tex_coord = vec2(v_tex_coord.x, v_tex_coord.y);
			}

			float y, u, v;
			if (u_base_textureIndex == 1) {
				y = texture2D(u_base_texture_y, tex_coord).b;
				u = texture2D(u_base_texture_u, tex_coord).b - 0.5;
				v = texture2D(u_base_texture_v, tex_coord).b - 0.5;
			}
			else if (u_base_textureIndex == 2) {
				y = texture2D(u_base_texture_y, tex_coord).g;
				u = texture2D(u_base_texture_u, tex_coord).g - 0.5;
				v = texture2D(u_base_texture_v, tex_coord).g - 0.5;
			}
			else {
				y = texture2D(u_base_texture_y, tex_coord).r;
				u = texture2D(u_base_texture_u, tex_coord).r - 0.5;
				v = texture2D(u_base_texture_v, tex_coord).r - 0.5;
			}

			float r = y + 1.402 * v;
			float g = y - 0.344 * u - 0.714 * v;
			float b = y + 1.772 * u;

			base_color = vec4(r, g, b, 1.0);

		}

		if (u_flag_texture_diffrence == false) {
			final_color = base_color;
		}
		else {
			//get original texture color and get diffrence
			vec4 original_color;

			vec2 tex_coord2;
			tex_coord2 = vec2(v_tex_coord.x, 1 - v_tex_coord.y);
			if (u_type_of_original_image == 0) {//rgb image
				original_color = texture(u_original_texture, tex_coord2);
			}
			else if (u_type_of_original_image == 1) {//yuv image

				float y, u, v;
				if (u_original_textureIndex == 1) {
					y = texture2D(u_original_texture_y, tex_coord2).b;
					u = texture2D(u_original_texture_u, tex_coord2).b - 0.5;
					v = texture2D(u_original_texture_v, tex_coord2).b - 0.5;
				}
				else if (u_original_textureIndex == 2) {
					y = texture2D(u_original_texture_y, tex_coord2).g;
					u = texture2D(u_original_texture_u, tex_coord2).g - 0.5;
					v = texture2D(u_original_texture_v, tex_coord2).g - 0.5;
				}
				else {
					y = texture2D(u_original_texture_y, tex_coord2).r;
					u = texture2D(u_original_texture_u, tex_coord2).r - 0.5;
					v = texture2D(u_original_texture_v, tex_coord2).r - 0.5;
				}

				float r = y + 1.402 * v;
				float g = y - 0.344 * u - 0.714 * v;
				float b = y + 1.772 * u;

				original_color = vec4(r, g, b, 1.0);
			}

			final_color.x = 1.0 - abs(original_color.x - base_color.x);
			final_color.y = 1.0 - abs(original_color.y - base_color.y);
			final_color.z = 1.0 - abs(original_color.z - base_color.z);
		}
	}
}
