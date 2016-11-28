/*
 *  linux/drivers/devfreq/governor_simpleondemand.c
 *
 *  Copyright (C) 2011 Samsung Electronics
 *	MyungJoo Ham <myungjoo.ham@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/errno.h>
#include <linux/module.h>
#include <linux/devfreq.h>
#include <linux/math64.h>
#include "governor.h"

#define DEVFREQ_SIMPLE_ONDEMAND	"simple_ondemand"

<<<<<<< HEAD
/* Default constants for DevFreq-Simple-Ondemand (DFSO) */
#define DFSO_UPTHRESHOLD	60
#define DFSO_DOWNDIFFERENCTIAL	20

static unsigned int dfso_upthreshold = DFSO_UPTHRESHOLD;
static unsigned int dfso_downdifferential = DFSO_DOWNDIFFERENCTIAL;


=======
>>>>>>> 4e32c4121f2e0d83ffd2dc980b909cad291501cc
static int devfreq_simple_ondemand_func(struct devfreq *df,
					unsigned long *freq,
					u32 *flag)
{
	struct devfreq_dev_status stat;
<<<<<<< HEAD
=======
	int err;
	unsigned long long a, b;
>>>>>>> 4e32c4121f2e0d83ffd2dc980b909cad291501cc
	struct devfreq_simple_ondemand_data *data = df->data;
	int err;
	unsigned long long a, b;
	unsigned long max = (df->max_freq) ? df->max_freq : UINT_MAX;
	unsigned long min = (df->min_freq) ? df->min_freq : 0;

<<<<<<< HEAD
=======
	if (!data)
		return -EINVAL;

>>>>>>> 4e32c4121f2e0d83ffd2dc980b909cad291501cc
	stat.private_data = NULL;

	err = df->profile->get_dev_status(df->dev.parent, &stat);
	if (err)
		return err;

	/* Prevent overflow */
	if (stat.busy_time >= (1 << 24) || stat.total_time >= (1 << 24)) {
		stat.busy_time >>= 7;
		stat.total_time >>= 7;
	}

	if (data && data->simple_scaling) {
		if (stat.busy_time * 100 >
		    stat.total_time * data->upthreshold)
			*freq = max;
		else if (stat.busy_time * 100 <
		    stat.total_time * data->downdifferential)
			*freq = min;
		else
			*freq = df->previous_freq;
		return 0;
	}

	/* Assume MAX if it is going to be divided by zero */
	if (stat.total_time == 0) {
		*freq = max;
		return 0;
	}

	/* Set MAX if it's busy enough */
	if (stat.busy_time * 100 >
	    stat.total_time * data->upthreshold) {
		*freq = max;
		return 0;
	}

	/* Set MAX if we do not know the initial frequency */
	if (stat.current_frequency == 0) {
		*freq = max;
		return 0;
	}

	/* Keep the current frequency */
	if (stat.busy_time * 100 >
	    stat.total_time * (data->upthreshold - data->downdifferential)) {
		*freq = stat.current_frequency;
		return 0;
	}

	/* Set the desired frequency based on the load */
	a = stat.busy_time;
	a *= stat.current_frequency;
	b = div_u64(a, stat.total_time);
	b *= 100;
	b = div_u64(b, (data->upthreshold - data->downdifferential / 2));
	*freq = (unsigned long) b;

	if (df->min_freq && *freq < df->min_freq)
		*freq = df->min_freq;
	if (df->max_freq && *freq > df->max_freq)
		*freq = df->max_freq;

	return 0;
}

<<<<<<< HEAD
static ssize_t simple_ondemand_upthreshold_show(struct kobject *kobj,
						struct kobj_attribute *attr,
						char *buf)
{
	return sprintf(buf, "%d\n", dfso_upthreshold);
}

static ssize_t simple_ondemand_upthreshold_store(struct kobject *kobj,
						  struct kobj_attribute *attr,
						  const char *buf, size_t count)
{
	unsigned int val;

	sscanf(buf, "%d", &val);
	if (val > 100 || val < dfso_downdifferential)
		return -EINVAL;

	dfso_upthreshold = val;
=======
static ssize_t upthreshold_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct devfreq *devfreq = to_devfreq(dev);
	struct devfreq_simple_ondemand_data *data = devfreq->data;

	return sprintf(buf, "%d\n", data->upthreshold);
}

static ssize_t upthreshold_store(struct device *dev,
				 struct device_attribute *attr, const char *buf,
				 size_t count)
{
	unsigned int val;
	struct devfreq *devfreq = to_devfreq(dev);
	struct devfreq_simple_ondemand_data *data = devfreq->data;

	sscanf(buf, "%d", &val);
	if (val > 100 || val < data->downdifferential)
		return -EINVAL;

	data->upthreshold = val;
>>>>>>> 4e32c4121f2e0d83ffd2dc980b909cad291501cc

	return count;
}

<<<<<<< HEAD
static ssize_t simple_ondemand_downdifferential_show(struct kobject *kobj,
						     struct kobj_attribute *attr,
						     char *buf)
{
	return sprintf(buf, "%d\n", dfso_downdifferential);
}

static ssize_t simple_ondemand_downdifferential_store(struct kobject *kobj,
						      struct kobj_attribute *attr,
						      const char *buf, size_t count)
{
	unsigned int val;

	sscanf(buf, "%d", &val);
	if (val > dfso_upthreshold)
		return -EINVAL;

	dfso_downdifferential = val;
=======
static ssize_t downdifferential_show(struct device *dev,
				     struct device_attribute *attr, char *buf)
{
	struct devfreq *devfreq = to_devfreq(dev);
	struct devfreq_simple_ondemand_data *data = devfreq->data;

	return sprintf(buf, "%d\n", data->downdifferential);
}

static ssize_t downdifferential_store(struct device *dev,
				      struct device_attribute *attr,
				      const char *buf, size_t count)
{
	unsigned int val;
	struct devfreq *devfreq = to_devfreq(dev);
	struct devfreq_simple_ondemand_data *data = devfreq->data;

	sscanf(buf, "%d", &val);
	if (val > data->upthreshold)
		return -EINVAL;

	data->downdifferential = val;
>>>>>>> 4e32c4121f2e0d83ffd2dc980b909cad291501cc

	return count;
}

<<<<<<< HEAD
static struct kobj_attribute upthreshold_attribute =
	__ATTR(upthreshold, 0664, simple_ondemand_upthreshold_show,
	       simple_ondemand_upthreshold_store);
static struct kobj_attribute downdifferential_attribute =
	__ATTR(downdifferential, 0664, simple_ondemand_downdifferential_show,
	       simple_ondemand_downdifferential_store);

static struct attribute *attrs[] = {
	&upthreshold_attribute.attr,
	&downdifferential_attribute.attr,
=======
static DEVICE_ATTR(upthreshold, 0644, upthreshold_show, upthreshold_store);
static DEVICE_ATTR(downdifferential, 0644, downdifferential_show,
		   downdifferential_store);

static struct attribute *attrs[] = {
	&dev_attr_upthreshold.attr,
	&dev_attr_downdifferential.attr,
>>>>>>> 4e32c4121f2e0d83ffd2dc980b909cad291501cc
	NULL,
};

static struct attribute_group attr_group = {
	.attrs = attrs,
	.name = DEVFREQ_SIMPLE_ONDEMAND,
};

static int devfreq_simple_ondemand_handler(struct devfreq *devfreq,
				unsigned int event, void *data)
{
	int ret = 0;

	switch (event) {
	case DEVFREQ_GOV_START:
		devfreq_monitor_start(devfreq);
		ret = devfreq_policy_add_files(devfreq, attr_group);
		break;

	case DEVFREQ_GOV_STOP:
		devfreq_policy_remove_files(devfreq, attr_group);
		devfreq_monitor_stop(devfreq);
		break;

	case DEVFREQ_GOV_INTERVAL:
		devfreq_interval_update(devfreq, (unsigned int *)data);
		break;

	case DEVFREQ_GOV_SUSPEND:
		devfreq_monitor_suspend(devfreq);
		break;

	case DEVFREQ_GOV_RESUME:
		devfreq_monitor_resume(devfreq);
		break;

	default:
		break;
	}

	return ret;
}

static struct devfreq_governor devfreq_simple_ondemand = {
	.name = DEVFREQ_SIMPLE_ONDEMAND,
	.get_target_freq = devfreq_simple_ondemand_func,
	.event_handler = devfreq_simple_ondemand_handler,
};

static int __init devfreq_simple_ondemand_init(void)
{
	return devfreq_add_governor(&devfreq_simple_ondemand);
}
subsys_initcall(devfreq_simple_ondemand_init);

static void __exit devfreq_simple_ondemand_exit(void)
{
	int ret;

	ret = devfreq_remove_governor(&devfreq_simple_ondemand);
	if (ret)
		pr_err("%s: failed remove governor %d\n", __func__, ret);

	return;
}
module_exit(devfreq_simple_ondemand_exit);
MODULE_LICENSE("GPL");
