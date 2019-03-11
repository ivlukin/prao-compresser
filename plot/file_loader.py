# -*- coding: utf-8 -*-
import array
import os
import time
import logging
import pandas as pd

# 1. загрузить полный df из файла


# 2. отфильтровать по определенной частоте
# 3. слайдер для каналов


def load(path):
    with open(path, 'rb') as fp:
        data = fp.read()
        numpar = int(data.split('\r\n')[0].split('\t')[1])

        header_length = sum([len(data.split('\r\n')[i]) + 2 for i in range(numpar + 1)])

        del data

    time_started = time.time()
    as_float_array = array.array('f')

    file_length = os.path.getsize(path) - header_length

    logging.debug('File size w/o header is {} B'.format(file_length))

    with open(path, 'rb') as fp:
        header_raw = fp.read(header_length)
        header = dict(map(lambda x: (x.split('\t')[0], x.split('\t')[1]), filter(len, header_raw.split('\r\n'))))
        header['npoints'] = int(header['npoints'])
        header['nmetrics'] = 9  # TODO:
        header['nrays'] = 48  # TODO:
        header['nbands'] = 33  # TODO:

        fp.seek(header_length)
        as_float_array.fromfile(fp, file_length / 4)

    struct = {
        'metric_num': [None] * len(as_float_array),  # индекс метрики
        'band_num': [None] * len(as_float_array),  # индекс частоты
        'ray_num': [None] * len(as_float_array),  # индекс луча
        'ts': [None] * len(as_float_array),  # условный индекс времени
    }

    # logging.debug('{0}\t{1}\t{2}\t{3}'.format('metric', 'band', 'ray', 'time'))

    floats = len(as_float_array)
    for i in range(floats):
        metric_num = i / (header['nbands'] * header['nrays'] * header['npoints'])

        yesterday_all_my_troubles = i % (header['nbands'] * header['nrays'] * header['npoints'])
        band_num = yesterday_all_my_troubles / (header['nrays'] * header['npoints'])

        seemed_so_far_away = yesterday_all_my_troubles % (header['nrays'] * header['npoints'])
        ray_num = seemed_so_far_away / header['npoints']

        ts = seemed_so_far_away % header['npoints']

        struct['metric_num'][i] = metric_num
        struct['band_num'][i] = band_num
        struct['ray_num'][i] = ray_num
        struct['ts'][i] = ts

    struct['value'] = as_float_array

    time_finished = time.time()

    logging.debug('{0:.2f}s elapsed'.format(time_finished - time_started))

    return pd.DataFrame(struct)
