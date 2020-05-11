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

uniform int u_depth_version;
uniform int u_depth_number;
uniform sampler2D u_depth_uncomp_16bit;
uniform sampler2D u_depth_uncomp_upper;
uniform sampler2D u_depth_uncomp_lower;
uniform sampler2D u_depth_comp_upper;
uniform sampler2D u_depth_comp_lower;
uniform sampler2D u_depth_comp_split1;
uniform sampler2D u_depth_comp_split2;

uniform bool u_flag_texture_Depth_of_color = false;
uniform bool u_flag_texture_reverse = false;
uniform bool u_flag_texture_mapping = true;
uniform bool u_flag_texture_diffrence = false;

in vec3 v_position_EC;
in vec3 v_normal_EC;
in vec2 v_tex_coord;

layout (location = 0) out vec4 final_color;

vec4 getDepthColor(int depth_version) {

	vec4 depth_color;
	vec4 upper_color, lower_color;
	vec2 coord = vec2(v_tex_coord.x, 1 - v_tex_coord.y);
	float gray_color;

	int color;
	int color_r;
	int color_g;
	int color_b;
	int color_a;
	int color_num = u_depth_number%4;


	switch (depth_version) {
	case 0: //uncompressed
		depth_color = texture(u_depth_uncomp_16bit, coord);

		switch (color_num) {
		case 0: //r
			gray_color = depth_color.r;
			break;
		case 1: //g
			gray_color = depth_color.g;
			break;
		case 2: //b
			gray_color = depth_color.b;
			break;
		case 3: //a
			gray_color = depth_color.a;
			break;
		}

		break;

	case 1://uncomp up - BPTC low
		upper_color = texture(u_depth_uncomp_upper, coord);
		lower_color = texture(u_depth_comp_lower, v_tex_coord);

		switch (color_num) {
		case 0: //r
			gray_color = upper_color.r + lower_color.r / 256.0;
			break;
		case 1: //g
			gray_color = upper_color.g + lower_color.g / 256.0;
			break;
		case 2: //b
			gray_color = upper_color.b + lower_color.b / 256.0;
			break;
		case 3: //a
			gray_color = upper_color.a + lower_color.a / 256.0;
			break;
		}
		break;


	case 2://BPTC up - uncomp low
		upper_color = texture(u_depth_comp_upper, v_tex_coord);
		lower_color = texture(u_depth_uncomp_lower, coord);
		
		switch (color_num) {
		case 0: //r
			gray_color = upper_color.r + lower_color.r / 256.0;
			break;
		case 1: //g
			gray_color = upper_color.g + lower_color.g / 256.0;
			break;
		case 2: //b
			gray_color = upper_color.b + lower_color.b / 256.0;
			break;
		case 3: //a
			gray_color = upper_color.a + lower_color.a / 256.0;
			break;
		}
		break;


	case 3://BPTC up - BPTC low
		upper_color = texture(u_depth_comp_upper, v_tex_coord);
		lower_color = texture(u_depth_comp_lower, v_tex_coord);

		switch (color_num) {
		case 0: //r
			gray_color = upper_color.r + lower_color.r / 256;
			break;
		case 1: //g
			gray_color = upper_color.g + lower_color.g / 256;
			break;
		case 2: //b
			gray_color = upper_color.b + lower_color.b / 256;
			break;
		case 3: //a
			gray_color = upper_color.a + lower_color.a / 256;
			break;
		}
		break;


	case 4: //dxt1_high fill
		depth_color = texture(u_depth_comp_split1, v_tex_coord);
		color_r = int(depth_color.r * 256);
		color_g = int(depth_color.g * 256);
		color_b = int(depth_color.b * 256);
		color = color_b / 8 + color_g * 8 + color_r * 256;
		gray_color = float(color) / 65536;
		break;

	case 5: //bptc_high fill
		depth_color = texture(u_depth_comp_split2, v_tex_coord);
		color_r = int(depth_color.r * 256);
		color_g = int(depth_color.g * 256);
		color_b = int(depth_color.b * 256);
		color_a = int(depth_color.a * 256);
		color = color_b / 16 + color_g + color_r * 16 + color_a * 256;
		gray_color = float(color) / 65536;

		//depth_color = texture(u_depth_comp_split2, v_tex_coord);
		break;

	case 99:
		upper_color = texture(u_depth_uncomp_upper, coord);
		lower_color = texture(u_depth_uncomp_lower, coord);

		switch (color_num) {
		case 0: //r
			color = int(upper_color.r * 65535) + int(lower_color.r * 256);
			gray_color = float(color) / 65536;
			break;
		case 1: //g
			gray_color = upper_color.g + lower_color.g / 256;
			break;
		case 2: //b
			gray_color = upper_color.b + lower_color.b / 256;
			break;
		case 3: //a
			gray_color = upper_color.a + lower_color.a / 256;
			break;
		}

		break;


	//case 25:
	//	depth_color = texture(u_depth_uncomp_split, coord);
	//	color_r = int(depth_color.r * 255);
	//	color_g = int(depth_color.g * 255);
	//	color_b = int(depth_color.b * 255);
	//	color = color_b + color_g * 32 + color_r * 2048;
	//	gray_color = float(color) / 65536;
	//	depth_color.r = gray_color;
	//	depth_color.g = gray_color;
	//	depth_color.b = gray_color;
	//	depth_color.a = 1.0;
	//	break;
	}

	depth_color.r = gray_color;
	depth_color.g = gray_color;
	depth_color.b = gray_color;
	depth_color.a = 1.0;

	return depth_color;
}

void printDepthMap() {

	vec4 depth_color = getDepthColor(u_depth_version);

	vec4 original_color = getDepthColor(99);

	//final_color.x = 1.0 - abs(depth_color.r);
	//final_color.y = 1;
	//final_color.z = 1;
	final_color = depth_color;

	if (u_flag_texture_diffrence == true) {
		float diff = abs(depth_color.r - original_color.r) * 20;
		if (diff > 1.0)
			diff = 1.0;
		final_color.x = 1.0 - diff;
		final_color.y = 1.0 - diff;
		final_color.z = 1.0 - diff;
	}
}

void main(void) {

	vec4 base_color;

	//render Depth Map Color
	if (u_flag_texture_Depth_of_color) {
		printDepthMap();
		return;
	}

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
