#include <linux/module.h>
#include <linux/platform_device.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/jack.h>

static int snd_tas5720_amp_init(struct snd_soc_pcm_runtime *rtd)
{
    return 0;
}

static int snd_tas5720_amp_hw_params(struct snd_pcm_substream *substream,
                                             struct snd_pcm_hw_params *params)
{
    struct snd_soc_pcm_runtime *rtd = substream->private_data;
    struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
    unsigned int sample_bits = snd_pcm_format_physical_width(params_format(params));

    printk("-------------> sample_bits: %d\n", sample_bits);

    return snd_soc_dai_set_bclk_ratio(cpu_dai, sample_bits * 2);
}

/* machine stream operations */
static struct snd_soc_ops snd_tas5720_amp_ops = {
    .hw_params = snd_tas5720_amp_hw_params,
};


static struct snd_soc_dai_link snd_tas5720_amp_dai_link[] = {
    {
    .name           = "TAS5720-MONO-AMP",
    .stream_name    = "TAS5720-MONO-AMP",
    .cpu_dai_name   = "c0055000.i2s",
    .codec_dai_name = "tas5720-amplifier",
    .platform_name  = "c0055000.i2s",
    .codec_name     = "tas5720",
    .dai_fmt        = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
                        SND_SOC_DAIFMT_CBM_CFM,
    .ops            = &snd_tas5720_amp_ops,
    .init           = snd_tas5720_amp_init,
    },
};

/* audio machine driver */
static struct snd_soc_card snd_tas5720_amp = {
    .name         = "I2S0-TAS5720",
    .owner        = THIS_MODULE,
    .num_links    = 1,
    .dai_link     = snd_tas5720_amp_dai_link,
    .num_links    = ARRAY_SIZE(snd_tas5720_amp_dai_link),
};

static const struct of_device_id snd_tas5720_amp_of_match[] = {
    { .compatible = "nexell,nexell-tas5720" },
    {},
};
MODULE_DEVICE_TABLE(of, snd_tas5720_amp_of_match);

static int snd_tas5720_amp_probe(struct platform_device *pdev)
{
    int ret = 0;
    printk("%s :%d HERE...\n",__func__,__LINE__);
    snd_tas5720_amp.dev = &pdev->dev;

    if (pdev->dev.of_node) {
        struct device_node *i2s_node;
        struct snd_soc_dai_link *dai = &snd_tas5720_amp_dai_link[0];
        i2s_node = of_parse_phandle(pdev->dev.of_node, "i2s-controller", 0);

        if (i2s_node) {
            dai->cpu_dai_name = NULL;
            dai->cpu_of_node = i2s_node;
            dai->platform_name = NULL;
            dai->platform_of_node = i2s_node;
        }
    }

    ret = snd_soc_register_card(&snd_tas5720_amp);
    if (ret)
    {
        printk("%s :%d HERE...\n",__func__,__LINE__);
        dev_err(&pdev->dev, "snd_soc_register_card() failed: %d\n", ret);
    }
    return ret;
}

static int snd_tas5720_amp_remove(struct platform_device *pdev)
{
    return snd_soc_unregister_card(&snd_tas5720_amp);
}

static struct platform_driver snd_tas5720_amp_asoc_driver = {
        .driver = {
                .name   = "tas5720-amp",
                .owner  = THIS_MODULE,
		.of_match_table = snd_tas5720_amp_of_match,
        },
        .probe          = snd_tas5720_amp_probe,
        .remove         = snd_tas5720_amp_remove,
};

module_platform_driver(snd_tas5720_amp_asoc_driver);

MODULE_AUTHOR("Phuc Le Minh <phuclm@lumi.com.vn>");
MODULE_DESCRIPTION("ASoC Driver for TAS5720 AMP");
MODULE_LICENSE("GPL v2");



