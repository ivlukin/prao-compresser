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
        numpar = int(data.split(b'\r\n')[0].split(b'\t')[1])
        logger.debug('numpar = %d', numpar)

        header_length = sum([len(data.split(b'\r\n')[i]) + 2 for i in range(numpar)])
        logger.debug('header_length = %d', header_length)

    time_started = time.time()
    as_float_array = array.array('f')

    file_length = os.path.getsize(path) - header_length

    logger.debug('File size w/o header is {} B'.format(file_length))

    with open(path, 'rb') as fp:
        header_raw = fp.read(header_length)
        _header = dict(map(lambda x: (x.split(b'\t')[0], x.split(b'\t')[1]), filter(len, header_raw.split(b'\r\n'))))
        header = {x.decode("utf-8") : _header[x] for x in _header}
        logging.debug(header)
        header['npoints'] = int(header['npoints'])
        header['nmetrics'] = 7
        header['nrays'] = 48
        header['nbands'] = int(header['nbands']) + 1
        #header['nbands'] = 33
        logger.debug(header)
        fp.seek(header_length)
        as_float_array.fromfile(fp, file_length // 4)

    npoints = header['npoints']
    nmetrics = header['nmetrics']
    nrays = header['nrays']
    nbands = header['nbands']

    nfloats = len(as_float_array)

    struct = {
        'ts': list(range(npoints)) * (nfloats // npoints),
        'ray_num': __join([[i] * npoints for i in range(nrays)]) * (nfloats // (nrays * npoints)),
        'metric_num': __join([[i] * (npoints * nrays) for i in range(nmetrics)]) * (nfloats // (nrays * npoints * nmetrics)),
        'band_num': __join([[i] * (npoints * nrays * nmetrics) for i in range(nbands)]),

        'value': as_float_array
    }

    for key in struct:
        logging.debug('%s %d', key, len(struct[key]))

    df = pd.DataFrame(struct)
    time_finished = time.time()

    logger.debug('{0:.2f}s elapsed'.format(time_finished - time_started))

    return df
