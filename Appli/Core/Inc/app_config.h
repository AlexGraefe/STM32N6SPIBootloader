 /**
 ******************************************************************************
 * @file    app_config.h
 * @author  GPM Application Team
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#ifndef APP_CONFIG
#define APP_CONFIG

#include "arm_math.h"

#define USE_DCACHE

/*Defines: CMW_MIRRORFLIP_NONE; CMW_MIRRORFLIP_FLIP; CMW_MIRRORFLIP_MIRROR; CMW_MIRRORFLIP_FLIP_MIRROR;*/
#define CAMERA_FLIP CMW_MIRRORFLIP_FLIP_MIRROR

#define ASPECT_RATIO_CROP       (1) /* Crop both pipes to nn input aspect ratio; Original aspect ratio kept */
#define ASPECT_RATIO_FIT        (2) /* Resize both pipe to NN input aspect ratio; Original aspect ratio not kept */
#define ASPECT_RATIO_FULLSCREEN (3) /* Resize camera image to NN input size and display a maximized image. See Doc/Build-Options.md#aspect-ratio-mode */
#define ASPECT_RATIO_MODE ASPECT_RATIO_FIT // ASPECT_RATIO_CROP

/* Model Related Info */
#define POSTPROCESS_TYPE    POSTPROCESS_OD_ST_YOLOX_UI

#define COLOR_BGR (0)
#define COLOR_RGB (1)
#define COLOR_MODE    COLOR_RGB

/* Runtime / HW settings */
#define APP_CAMERA_FPS                     (30)
#define APP_VENC_WIDTH                     (144)    // (144)
#define APP_VENC_HEIGHT                    (144)    // (96)
#define APP_VENC_FRAMERATE                 (APP_CAMERA_FPS)
#define APP_SECONDARY_PIPE_BPP             (2)
#define APP_SECONDARY_PIPE_BUFFER_SIZE     (APP_VENC_WIDTH * APP_VENC_HEIGHT * APP_SECONDARY_PIPE_BPP)
#define APP_VENC_OUTPUT_BUFFER_WORDS       (APP_VENC_WIDTH * APP_VENC_HEIGHT / 8)
#define APP_IRIS_SPI_BAUDRATE_PRESCALER    SPI_BAUDRATEPRESCALER_16
#define APP_STREAM_LOOP_DELAY_MS           (10)

/* SD Card */
#define SD_CARD_ENABLE 0
#define BLOCK_SIZE         (512) /* 1 Block = 512 Bytes */
#define NB_BLOCKS_TO_WRITE (256)  /* Write 128 Blocks at once*/
#define NB_BYTES_TO_WRITE (NB_BLOCKS_TO_WRITE*BLOCK_SIZE)
#define NB_WORDS_TO_WRITE (NB_BYTES_TO_WRITE/4)
#define NB_BLOCKS_ERASED  (1024*1024) /*Erase 512 MBytes = 512*1024*1024 */

#endif
