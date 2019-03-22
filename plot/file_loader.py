# -*- coding: utf-8 -*-
import array
import itertools
import os
import time
import logging
import pandas as pd

logger = logging.getLogger()


def __join(list_lists):
    return list(itertools.chain.from_iterable(list_lists))


def load(path):
    with open(path, 'rb') as fp:
        data = fp.read(1024)
        numpar = int(data.split('\r\n')[0].split('\t')[1])

        header_length = sum([len(data.split('\r\n')[i]) + 2 for i in range(numpar + 1)])

    time_started = time.time()
    as_float_array = array.array('f')

    file_length = os.path.getsize(path) - header_length

    logger.debug('File size w/o header is {} B'.format(file_length))

    with open(path, 'rb') as fp:
        header_raw = fp.read(header_length)
        header = dict(map(lambda x: (x.split('\t')[0], x.split('\t')[1]), filter(len, header_raw.split('\r\n'))))
        header['npoints'] = int(header['npoints'])
        header['nmetrics'] = 9  # TODO:
        header['nrays'] = 48  # TODO:
        header['nbands'] = 33  # TODO:
        logger.debug(header)
        fp.seek(header_length)
        as_float_array.fromfile(fp, file_length / 4)

    npoints = header['npoints']
    nmetrics = header['nmetrics']
    nrays = header['nrays']
    nbands = header['nbands']

    nfloats = len(as_float_array)

    struct = {
        'ts': list(range(npoints)) * (nfloats / npoints),
        'ray_num': __join([[i] * npoints for i in range(nrays)]) * (nfloats / (nrays * npoints)),
        'metric_num': __join([[i] * (npoints * nrays) for i in range(nmetrics)]) * (nfloats / (nrays * npoints * nmetrics)),
        'band_num': __join([[i] * (npoints * nrays * nmetrics) for i in range(nbands)]),

        'value': as_float_array
    }

    df = pd.DataFrame(struct)
    time_finished = time.time()

    logger.debug('{0:.2f}s elapsed'.format(time_finished - time_started))

    return df
