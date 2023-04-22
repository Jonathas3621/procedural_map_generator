#include <stdio.h>
#include <png.h>

int main(int argc, char **argv) {

    //Encerra o programa se a quantidade de argumentos da chamada for diferente de 4
    if(argc != 4) {
        fprintf(stderr, "Uso: %s <image1.png> <image2.png> <final_image.png>\n", argv[0]);
        return 1;
    }

    //Carrega a imagem 1 na memória
    FILE *file1 = fopen(argv[1], "rb");
    if(!file1) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", argv[1]);
        return 1;
    }

    //Carrega as informações da imagem 1 em structs da LIBPNG
    png_structp png_ptr1 = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr1 = png_create_info_struct(png_ptr1);
    png_init_io(png_ptr1, file1);
    png_read_info(png_ptr1, info_ptr1);

    //Pega informações da imagem 1
    png_byte color_type1 = png_get_color_type(png_ptr1, info_ptr1);
    png_byte bit_depth1 = png_get_bit_depth(png_ptr1, info_ptr1);
    int width1 = png_get_image_width(png_ptr1, info_ptr1);
    int height1 = png_get_image_height(png_ptr1, info_ptr1);


    //Carrega o arquivo 2 na memória
    FILE *file2 = fopen(argv[2], "rb");
    if(!file2) {
        fprinf(stderr, "Erro ao abrir o arquivo %s\n", argv[2]);
        return 1;
    }

    //Carrega as informações da imagem 2 em structs da LIBPNG
    png_structp png_ptr2 = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr2 = png_create_info_struct(png_ptr2);
    png_init_io(png_ptr2, file2);
    png_read_info(png_ptr2, info_ptr2);

    //Pega informações da imagem 2
    png_byte color_type2 = png_get_color_type(png_ptr2, info_ptr2);
    png_byte bit_depth2 = png_get_bit_depth(png_ptr2, info_ptr2);
    int width2 = png_get_image_width(png_ptr2, info_ptr2);
    int height2 = png_get_image_height(png_ptr2, info_ptr2);

    //Fecha as imagens
    fclose(file1);
    fclose(file2);

    return 0;
}