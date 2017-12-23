/*
 * ASoC Driver for Microsemi DAC
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

#include <linux/module.h>
#include <linux/platform_device.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/jack.h>

static int snd_microsemi_dac_init(struct snd_soc_pcm_runtime *rtd)
{
    return 0;
}

static int snd_microsemi_dac_hw_params(struct snd_pcm_substream *substream,
                                             struct snd_pcm_hw_params *params)
{
    struct snd_soc_pcm_runtime *rtd = substream->private_data;
    struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
    unsigned int sample_bits = snd_pcm_format_physical_width(params_format(params));

    return snd_soc_dai_set_bclk_ratio(cpu_dai, sample_bits * 2);
}

/* machine stream operations */
static struct snd_soc_ops snd_microsemi_dac_ops = {
    .hw_params = snd_microsemi_dac_hw_params,
};
/* All of these can be made configurable through properties if using device tree.
TODO: re-design machine driver to take all relevant inputs i.e. master/slave, i2s mode,
        dai_link information from a device tree blob . Take reference of simple-card.c in
        /sound/soc/generic

 */
static struct snd_soc_dai_link snd_microsemi_dac_dai[] = {
    {
    .name           = "Microsemi DAC",
    .stream_name    = "Microsemi DAC ",
    .cpu_dai_name   = "c0056000.i2s",
    .codec_dai_name = "zl380xx-dai",
    .platform_name  = "c0056000.i2s",
    .codec_name     = "zl380-codec",
    .dai_fmt        = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
                        SND_SOC_DAIFMT_CBM_CFM,
    .ops            = &snd_microsemi_dac_ops,
    .init           = snd_microsemi_dac_init,
    },
};

/* audio machine driver */
static struct snd_soc_card snd_microsemi_dac = {
    .name         = "snd_microsemi_dac",
    .dai_link     = snd_microsemi_dac_dai,
    .num_links    = ARRAY_SIZE(snd_microsemi_dac_dai),
};

static int snd_microsemi_dac_probe(struct platform_device *pdev)
{
    int ret = 0;
    printk("%s :%d HERE...\n",__func__,__LINE__);
    snd_microsemi_dac.dev = &pdev->dev;

    if (pdev->dev.of_node) {
        struct device_node *i2s_node;
        struct snd_soc_dai_link *dai = &snd_microsemi_dac_dai[0];
        i2s_node = of_parse_phandle(pdev->dev.of_node,
        "i2s-controller", 0);

        if (i2s_node) {
            dai->cpu_dai_name = NULL;
            dai->cpu_of_node = i2s_node;
            dai->platform_name = NULL;
            dai->platform_of_node = i2s_node;
        }
    }

    ret = snd_soc_register_card(&snd_microsemi_dac);
    if (ret)
    {
        printk("%s :%d HERE...\n",__func__,__LINE__);
        dev_err(&pdev->dev, "snd_soc_register_card() failed: %d\n", ret);
    }
    return ret;
}

static int snd_microsemi_dac_remove(struct platform_device *pdev)
{
    return snd_soc_unregister_card(&snd_microsemi_dac);
}

static const struct of_device_id snd_microsemi_dac_of_match[] = {
    { .compatible = "microsemi,microsemi-dac", },
    {},
};
MODULE_DEVICE_TABLE(of, snd_microsemi_dac_of_match);

static struct platform_driver snd_microsemi_dac_asoc_driver = {
        .driver = {
                .name   = "snd-microsemi-dac",
                .owner  = THIS_MODULE,
		.of_match_table = snd_microsemi_dac_of_match,
        },
        .probe          = snd_microsemi_dac_probe,
        .remove         = snd_microsemi_dac_remove,
};

module_platform_driver(snd_microsemi_dac_asoc_driver);

MODULE_AUTHOR("Shally Verma <shally.verma@microsemi.com>");
MODULE_DESCRIPTION("ASoC Driver for Microsemi DAC");
MODULE_LICENSE("GPL v2");
