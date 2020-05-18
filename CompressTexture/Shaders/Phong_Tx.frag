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
uniform usampler2D u_depth_uncomp_16bit;
uniform usampler2D u_depth_uncomp_upper;
uniform usampler2D u_depth_uncomp_lower;
uniform sampler2D u_depth_comp_upper;
uniform sampler2D u_depth_comp_lower;
uniform usampler2D u_depth_comp_split1;
uniform usampler2D u_depth_comp_split2;

uniform bool u_flag_texture_Depth_of_color = false;
uniform bool u_flag_texture_reverse = false;
uniform bool u_flag_texture_mapping = true;
uniform bool u_flag_texture_diffrence = false;

in vec3 v_position_EC;
in vec3 v_normal_EC;
in vec2 v_tex_coord;

layout (location = 0) out vec4 final_color;

vec4 getDepthColor(int depth_version) {

	uvec4 depth_color;
	uvec4 upper_color, lower_color;
	vec4 upper_comp_color,lower_comp_color;
	vec2 coord = vec2(v_tex_coord.x, 1 - v_tex_coord.y);
	unsigned int gray_color=0;
	float result_color = 0;
	float tmp;
	unsigned int color;
	unsigned int color_r;
	unsigned int color_g;
	unsigned int color_b;
	unsigned int color_a;
	unsigned int color_num = u_depth_number%4;


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
		result_color = float(gray_color) / 65535;
		break;

	case 1://uncomp up - BPTC low
		upper_color = texture(u_depth_uncomp_upper, coord);
		lower_comp_color = texture(u_depth_comp_lower, v_tex_coord);

		switch (color_num) {
		case 0: //r
			tmp = lower_comp_color.r * 255;
			gray_color = (upper_color.b * 256 + unsigned int(tmp));
			break;
		case 1: //g
			tmp = lower_comp_color.g * 255;
			gray_color = (upper_color.g * 256 + unsigned int(tmp));
			break;
		case 2: //b
			tmp = lower_comp_color.b * 255;
			gray_color = (upper_color.r * 256 + unsigned int(tmp));
			break;
		case 3: //a
			tmp = lower_comp_color.a * 255;
			gray_color = (upper_color.a * 256 + unsigned int(tmp));
			break;
		}
		result_color = float(gray_color) / 65535;
		break;

	case 2://BPTC up - uncomp low
		upper_comp_color = texture(u_depth_comp_upper, v_tex_coord);
		lower_color = texture(u_depth_uncomp_lower, coord);
		
		switch (color_num) {
		case 0: //r
			tmp = upper_comp_color.r * 255;
			gray_color = unsigned int(tmp) * 256 + lower_color.b;
			break;
		case 1: //g
			tmp = upper_comp_color.g * 255;
			gray_color = unsigned int(tmp) * 256 + lower_color.g;
			break;
		case 2: //b
			tmp = upper_comp_color.b * 255;
			gray_color = unsigned int(tmp) * 256 + lower_color.r;
			break;
		case 3: //a
			tmp = upper_comp_color.a * 255;
			gray_color = unsigned int(tmp) * 256 + lower_color.a;
			break;
		}
		result_color = float(gray_color) / 65535;
		break;


	case 3://BPTC up - BPTC low
		upper_comp_color = texture(u_depth_comp_upper, v_tex_coord);
		lower_comp_color = texture(u_depth_comp_lower, v_tex_coord);


		switch (color_num) {
		case 0: //r
			tmp = upper_comp_color.r * 255;
			gray_color = unsigned int(tmp) * 256 + unsigned int(lower_comp_color.r*255);
			break;
		case 1: //g
			tmp = upper_comp_color.g * 255;
			gray_color = unsigned int(tmp) * 256 + unsigned int(lower_comp_color.g * 255);
			break;
		case 2: //b
			tmp = upper_comp_color.b * 255;
			gray_color = unsigned int(tmp) * 256 + unsigned int(lower_comp_color.b * 255);
			break;
		case 3: //a
			tmp = upper_comp_color.a * 255;
			gray_color = unsigned int(tmp) * 256 + unsigned int(lower_comp_color.a * 255);
			break;
		}
		result_color = float(gray_color) / 65535;
		break;


	case 4: //dxt1_high fill
		depth_color = texture(u_depth_comp_split1, v_tex_coord);
		color_r = (depth_color.r * 256);
		color_g = (depth_color.g * 256);
		color_b = (depth_color.b * 256);
		color = color_b / 8 + color_g * 8 + color_r * 256;
		gray_color = color;
		break;

	case 5: //bptc_high fill
		depth_color = texture(u_depth_comp_split2, v_tex_coord);
		color_r = (depth_color.r * 256);
		color_g = (depth_color.g * 256);
		color_b = (depth_color.b * 256);
		color_a = (depth_color.a * 256);
		color = color_b / 16 + color_g + color_r * 16 + color_a * 256;
		gray_color = color;
		break;


	case 99://uncomp up - uncomp low
		upper_color = texture(u_depth_uncomp_upper, coord);
		lower_color = texture(u_depth_uncomp_lower, coord);

		switch (color_num) {
		case 0: //r
			gray_color = (upper_color.b * 256 + lower_color.b);
			break;
		case 1: //g
			gray_color = (upper_color.g * 256 + lower_color.g);
			break;
		case 2: //b
			gray_color = (upper_color.r * 256 + lower_color.r);
			break;
		case 3: //a
			gray_color = (upper_color.a * 256 + lower_color.a);
			break;
		}
		result_color = float(gray_color) / 65535;
		break;


	}

	vec4 res_color;
	res_color.r = result_color;
	res_color.g = result_color;
	res_color.b = result_color;
	res_color.a = 1.0;
	return res_color;
}

void printDepthMap() {

	vec4 depth_color = getDepthColor(u_depth_version);
	if (u_drawtype == 0) {

		final_color = depth_color;

	}
	else {

		vec4 original_color = getDepthColor(0);

		final_color = depth_color;

		if (u_flag_texture_diffrence == true) {
			float diff = abs(depth_color.r - original_color.r) * 100;
			if (diff > 1.0)
				diff = 1.0;
			final_color.r = 1.0 - diff;
			final_color.g = 1.0 - diff;
			final_color.b = 1.0 - diff;
		}
	}
}

void main(void) {

	vec4 base_color;

	//render Depth Map Color
	if (u_flag_texture_Depth_of_color) {
		printDepthMap();
		return;
	}


	//final_color.r = 0;
	//final_color.g = 0;
	//final_color.b = 2137483648;
	//return;

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


			//test code
			//vec4 yr, ug, vb,ka;
			//vec4 ori;
			//ori = texture2D(u_original_texture_y, tex_coord);
			//yr = texture2D(u_base_texture_y, tex_coord);
			//ug = texture2D(u_base_texture_u, tex_coord);
			//vb = texture2D(u_base_texture_v, tex_coord);
			//ka = texture2D(u_original_texture_u, tex_coord);
			//float c = ori.a;
			//final_color = vec4(c, c, c, 1);
			////final_color = ka;
			////return;
			//if (ori.r != yr.r) {
			//	final_color = vec4(0.8, 0.5, 0.5, 1.0);
			//	return;
			//}
			//else if (ori.g != ug.g) {
			//	final_color = vec4(0.5, 0.8, 0.5, 1.0);
			//	return;
			//}
			//else if (ori.b != vb.b) {
			//	final_color = vec4(0.5, 0.5, 0.8, 1.0);
			//	return;
			//}
			//else if (ori.a != ka.a) {
			//	final_color = vec4(0.1, 0.1, 0.1, 1.0);
			//	return;
			//}
			//else {
			//	final_color = vec4(0.5, 0.5, 0.5, 1.0);
			//	return;

			//}



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

			final_color.x = 1.0 - abs(original_color.x - base_color.x)*20;
			final_color.y = 1.0 - abs(original_color.y - base_color.y)*20;
			final_color.z = 1.0 - abs(original_color.z - base_color.z)*20;

			float a;

			final_color.x = original_color.x / 2;
			final_color.y = original_color.y / 2;
			final_color.z = original_color.z / 2;

			final_color.x = base_color.x / 2;
			final_color.y = base_color.y / 2;
			final_color.z = base_color.z / 2;

			final_color.x = 1.0 - abs(original_color.x - base_color.x) * 20;
			final_color.y = 1.0 - abs(original_color.y - base_color.y) * 20;
			final_color.z = 1.0 - abs(original_color.z - base_color.z) * 20;

		}
	}
}
