#include <stdio.h>
#include <stdlib.h>
#include <png.h>

int main(int argc, char **argv) {

    //Encerra o programa se a quantidade de argumentos da chamada for diferente de 4
    if(argc != 4) {
        fprintf(stderr, "Uso: %s <image1.png> <image2.png> <final_image.png>\n", argv[0]);
        return 1;
    }

    //Carrega a imagem 1 na memória
    //FILE *file1 = fopen(argv[1], "rb");
    FILE *file1 = fopen("tiles\\2.png", "rb");
    if(!file1) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", argv[1]);
        return 1;
    }

    //Carrega as informações da imagem 1 em structs da LIBPNG
    png_structp png_ptr1 = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr1) {
        fprintf(stderr, "Erro ao criar struct de leitura da imagem.\n");
        return 1;
    }
    png_infop info_ptr1 = png_create_info_struct(png_ptr1);
    if(!info_ptr1) {
        png_destroy_read_struct(&png_ptr1, NULL, NULL);
        fprintf(stderr, "Erro ao criar struct de informações da imagem.\n");
        return 1;
    }
    png_init_io(png_ptr1, file1);
    png_read_info(png_ptr1, info_ptr1);

    //Pega informações da imagem 1
    png_byte color_type1 = png_get_color_type(png_ptr1, info_ptr1);
    png_byte bit_depth1 = png_get_bit_depth(png_ptr1, info_ptr1);
    int width1 = png_get_image_width(png_ptr1, info_ptr1);
    int height1 = png_get_image_height(png_ptr1, info_ptr1);

    //Coloca a imagem 1 no padrão de trabalho
    if(bit_depth1 == 16) {
        png_set_strip_16(png_ptr1);
    }
    if(color_type1 == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr1);
    }
    if(color_type1 == PNG_COLOR_TYPE_GRAY && bit_depth1 < 8) {
        png_set_expand_gray_1_2_4_to_8(png_ptr1);
    }
    if(png_get_valid(png_ptr1, info_ptr1, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png_ptr1);
    }
    if(color_type1 == PNG_COLOR_TYPE_RGB ||
        color_type1 == PNG_COLOR_TYPE_GRAY ||
        color_type1 == PNG_COLOR_TYPE_PALETTE) {
        png_set_filler(png_ptr1, 0xff, PNG_FILLER_AFTER);
    }
    if(color_type1 == PNG_COLOR_TYPE_GRAY ||
        color_type1 == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png_ptr1);
    }
    png_read_update_info(png_ptr1, info_ptr1);

    //Armazena as informações dos pixels da imagem 1
    png_bytep *row_pointers1 = (png_bytep*) malloc(sizeof(png_bytep) *height1);
    for(int y = 0; y < height1; y++) {
        row_pointers1[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr1, info_ptr1));
    }
    png_read_image(png_ptr1, row_pointers1);

    //Fecha imagem 1
    fclose(file1);

    //Carrega a imagem 2 na memória
    //FILE *file2 = fopen(argv[2], "rb");
    FILE *file2 = fopen("tiles\\3.png", "rb");
    if(!file2) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", argv[2]);
        return 1;
    }

    //Carrega as informações da imagem 2 em structs da LIBPNG
    png_structp png_ptr2 = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr2) {
        fprintf(stderr, "Erro ao criar struct de leitura da imagem.\n");
        return 1;
    }
    png_infop info_ptr2 = png_create_info_struct(png_ptr2);
    if(!info_ptr2) {
        png_destroy_read_struct(&png_ptr2, NULL, NULL);
        fprintf(stderr, "Erro ao criar struct de informações da imagem.\n");
        return 1;
    }
    png_init_io(png_ptr2, file2);
    png_read_info(png_ptr2, info_ptr2);

    //Pega informações da imagem 2
    png_byte color_type2 = png_get_color_type(png_ptr2, info_ptr2);
    png_byte bit_depth2 = png_get_bit_depth(png_ptr2, info_ptr2);
    int width2 = png_get_image_width(png_ptr2, info_ptr2);
    int height2 = png_get_image_height(png_ptr2, info_ptr2);

     //Coloca a imagem 2 no padrão de trabalho
    if(bit_depth2 == 16) {
        png_set_strip_16(png_ptr2);
    }
    if(color_type2 == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr2);
    }
    if(color_type2 == PNG_COLOR_TYPE_GRAY && bit_depth2 < 8) {
        png_set_expand_gray_1_2_4_to_8(png_ptr2);
    }
    if(png_get_valid(png_ptr2, info_ptr2, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png_ptr2);
    }
    if(color_type2 == PNG_COLOR_TYPE_RGB ||
        color_type2 == PNG_COLOR_TYPE_GRAY ||
        color_type2 == PNG_COLOR_TYPE_PALETTE) {
        png_set_filler(png_ptr2, 0xff, PNG_FILLER_AFTER);
    }
    if(color_type2 == PNG_COLOR_TYPE_GRAY ||
        color_type2 == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png_ptr2);
    }
    png_read_update_info(png_ptr2, info_ptr2);

    //Armazena as informações dos pixels da imagem 2
    png_bytep *row_pointers2 = (png_bytep*) malloc(sizeof(png_bytep) * height2);
    for(int y = 0; y < height2; y++) {
        row_pointers2[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr2, info_ptr2));
    }
    png_read_image(png_ptr2, row_pointers2);

    //Fecha imagem 2
    fclose(file2);

    //Cria a nova imagem
    int new_width = width1 + width2;
    int new_height = height1 > height2 ? height1 : height2;
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr) {
        fprintf(stderr, "Erro na criação da estrutura de escrita.");

        for(int y = 0; y < height1; y++) {
            free(row_pointers1[y]);
        }
        for(int y = 0; y < height2; y++) {
            free(row_pointers2[y]);
        }

        free(row_pointers1);
        free(row_pointers2);
        png_destroy_read_struct(&png_ptr1, &info_ptr1, NULL);
        png_destroy_read_struct(&png_ptr2, &info_ptr2, NULL);

        return 1;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr) {
        fprintf(stderr, "Erro na criação da estrutura de escrita");

        for(int y = 0; y < height1; y++) {
            free(row_pointers1[y]);
        }
        for(int y = 0; y < height2; y++) {
            free(row_pointers2[y]);
        }

        free(row_pointers1);
        free(row_pointers2);
        png_destroy_read_struct(&png_ptr1, &info_ptr1, NULL);
        png_destroy_read_struct(&png_ptr2, &info_ptr2, NULL);

        return 1;
    }
    png_byte color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    png_byte bit_depth = 8;
    png_set_IHDR(png_ptr, info_ptr, new_width, new_height, bit_depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    //Criando matriz de pixels
    png_bytep *row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * new_height);
    for(int y = 0; y < new_height; y++) {
        row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr, info_ptr));
    }

    //Copiando as linhas da imagem 1 para a nova imagem
    for(int y = 0; y < height1; y++) {
        png_bytep row = row_pointers[y];
        for(int x = 0; x < width1; x++) {
            png_bytep px = &(row[x * 4]);
            px[0] = row_pointers1[y][x * 4];
            px[1] = row_pointers1[y][x * 4 + 1];
            px[2] = row_pointers1[y][x * 4 + 2];
            px[3] = row_pointers1[y][x * 4 + 3];
        }
    }

    //Copiando as linhas da imagem 2 para a nova imagem
    for(int y = 0; y < height2; y++) {
        png_bytep row = row_pointers[y];
        for(int x = 0; x < width2; x++) {
            png_bytep px = &(row[(width1 + x) * 4]);
            px[0] = row_pointers2[y][x * 4];
            px[1] = row_pointers2[y][x * 4 + 1];
            px[2] = row_pointers2[y][x * 4 + 2];
            px[3] = row_pointers2[y][x * 4 + 3];
        }
    }

    //Escreve a nova imagem em um arquivo
    //FILE *fp = fopen(argv[3], "wb");
    FILE *fp = fopen("new.png", "wb");
    if(!fp) {
        fprintf(stderr, "Erro ao criar o arquivo %s", argv[3]);

        //Libera memória alocada
        for(int y = 0; y < height1; y++) {
            free(row_pointers1[y]);
        }
        for(int y = 0; y < height2; y++) {
            free(row_pointers2[y]);
        }
        for(int y = 0; y < new_height; y++) {
            free(row_pointers[y]);
        }

        free(row_pointers1);
        free(row_pointers2);
        free(row_pointers);
        png_destroy_read_struct(&png_ptr1, &info_ptr1, NULL);
        png_destroy_read_struct(&png_ptr2, &info_ptr2, NULL);
        png_destroy_write_struct(&png_ptr, &info_ptr);

        return 1;
    }
    png_init_io(png_ptr, fp);
    png_write_info(png_ptr, info_ptr);
    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, NULL);

    //Libera memória alocada
    for(int y = 0; y < height1; y++) {
        free(row_pointers1[y]);
    }
    for(int y = 0; y < height2; y++) {
        free(row_pointers2[y]);
    }
    for(int y = 0; y < new_height; y++) {
        free(row_pointers[y]);
    }

    free(row_pointers1);
    free(row_pointers2);
    free(row_pointers);
    png_destroy_read_struct(&png_ptr1, &info_ptr1, NULL);
    png_destroy_read_struct(&png_ptr2, &info_ptr2, NULL);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    //Fecha a nova imagem
    fclose(fp);

    printf("All right\n");

    return 0;
}