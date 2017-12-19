/*
 * Driver for the ZL380xx codec
 *
 * Copyright (c) 2016, Microsemi Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#include <sound/soc.h>

static struct snd_soc_dai_driver zl380xx_dai = {
    .name = "zl380xx-dai",
    .playback = {
        .channels_min = 1,
        .channels_max = 2,
        .rates = (SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_16000 | SNDRV_PCM_RATE_48000 | SNDRV_PCM_RATE_44100),
        .formats = SNDRV_PCM_FMTBIT_S16_LE
    },
    .capture = {
        .channels_min = 1,
        .channels_max = 2,
        .rates = (SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_16000 |SNDRV_PCM_RATE_48000),
        .formats = SNDRV_PCM_FMTBIT_S16_LE
    }
};

static struct snd_soc_codec_driver soc_codec_dev_zl380xx;

static int zl380xx_probe(struct platform_device *pdev)
{
    return snd_soc_register_codec(&pdev->dev, &soc_codec_dev_zl380xx,&zl380xx_dai, 1);
}

static int zl380xx_remove(struct platform_device *pdev)
{
    snd_soc_unregister_codec(&pdev->dev);
    return 0;
}

static const struct of_device_id zl380xx_of_match[] = {
    { .compatible = "ms,zl38040", },
    { .compatible = "ms,zl38050", },
    { .compatible = "ms,zl38051", },
    { .compatible = "ms,zl38060", },
    { .compatible = "ms,zl38080", },
    {}
};
MODULE_DEVICE_TABLE(of, zl380xx_of_match);

static struct platform_driver zl380xx_codec_driver = {
    .probe      = zl380xx_probe,
    .remove     = zl380xx_remove,
    .driver = {
    .name   = "zl380-codec",
    .owner  = THIS_MODULE,
    .of_match_table = zl380xx_of_match,
    },
};

module_platform_driver(zl380xx_codec_driver);

MODULE_DESCRIPTION("ASoC zl380xx codec driver");
MODULE_AUTHOR("Shally Verma");
MODULE_LICENSE("GPL v2");
