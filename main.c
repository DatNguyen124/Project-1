#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

unsigned char *uc_arrayNew_1d(int _size)
{
    return (unsigned char *)calloc(_size, sizeof(unsigned char));
}


unsigned char *replace_background(unsigned char *weather_forecast, unsigned char *background, unsigned char *foreground, int width, int height, int channel, int channel_fg_bg)
{
    unsigned char *rimage = uc_arrayNew_1d(width * height);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int diff_pixel = 0;
            for (int k = 0; k < channel; k++)
            {
                if (abs(foreground[i * width * channel_fg_bg + j * channel_fg_bg + k] - background[i * width * channel_fg_bg + j * channel_fg_bg + k]) < 50)
                {
                    diff_pixel++;
                }
            }
            if (diff_pixel < 3)
            {
                for (int n = 0; n < channel; n++)
                {
                    weather_forecast[i * width * channel + j * channel + n] = foreground[i * width * channel_fg_bg + j * channel_fg_bg + n];
                }
            }
        }
    }
    return weather_forecast;
}

int main()
{
    // declare variables
    int width, height, channel;
    int width_fg_bg, height_fg_bg, channel_fg_bg;

    char background_img[] = "./images/background.png";
    char foreground_img[] = "./images/foreground.png";
    char weather_forecast_img[] = "./images/weather_forecast.png";
    char save_path_result[] = "./images/result.png";

    // read image data
    unsigned char *background = stbi_load(background_img, &width_fg_bg, &height_fg_bg, &channel_fg_bg, 0);
    unsigned char *foreground = stbi_load(foreground_img, &width_fg_bg, &height_fg_bg, &channel_fg_bg, 0);
    unsigned char *weather_forecast = stbi_load(weather_forecast_img, &width, &height, &channel, 0);
    
    if (background == NULL || foreground == NULL || weather_forecast == NULL)
    {
        printf("\nError in loading the image\n");
        exit(1);
    }
    
    printf("Width = %d\nHeight = %d\nChannel = %d\n", width, height, channel);

    // replace background with weather_forecast background
    unsigned char *rimage = replace_background(weather_forecast, background, foreground, width, height, channel, channel_fg_bg);
    stbi_write_png(save_path_result, width, height, channel, rimage, width * channel);
    printf("New image saved to %s\n", save_path_result);
}