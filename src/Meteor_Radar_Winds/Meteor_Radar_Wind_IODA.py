#!/usr/bin/env python3

import netCDF4 as nc
import numpy as np
from datetime import datetime, timedelta

from ioda import ioda_obs_space as ioda_os
from ioda import ioda_vars as ioda_vars
from ioda import ioda_attr as ioda_attr


def read_input(ncfile):
    ds = nc.Dataset(ncfile, 'r')

    data = {}
    data['lat'] = ds.variables['latitude'][:]
    data['lon'] = ds.variables['longitude'][:]
    data['time'] = ds.variables['time'][:]

    data['u'] = ds.variables['u_wind'][:]   # zonal
    data['v'] = ds.variables['v_wind'][:]   # meridional

    ds.close()
    return data



def convert_time(seconds_since_epoch):
    epoch = datetime(1970, 1, 1)
    return np.array([
        epoch + timedelta(seconds=float(t))
        for t in seconds_since_epoch
    ], dtype='datetime64[s]')


def write_ioda(data, outfile):

    nlocs = len(data['wind'])

    # Create ObsSpace
    obs_space = ioda_os.ObsSpace(
        outfile,
        mode='w',
        dim_dict={'Location': nlocs}
    )


    meta = {
        'latitude': data['lat'],
        'longitude': data['lon'],
        'datetime': data['time'],
    }

    meta_vars = {}
    for name, values in meta.items():
        meta_vars[name] = ioda_vars.Variable(
            obs_space,
            name,
            values.dtype,
            ('Location',),
            fillval=None
        )
        meta_vars[name].write_data(values)


    # ---- ObsValue ----
    u_obs = ioda_vars.Variable(
        obs_space,
        'eastward_wind',
        np.float32,
        ('Location',),
        fillval=np.nan,
        group='ObsValue'
    )
    u_obs.write_data(data['u'])

    v_obs = ioda_vars.Variable(
        obs_space,
        'northward_wind',
        np.float32,
        ('Location',),
        fillval=np.nan,
        group='ObsValue'
    )
    v_obs.write_data(data['v'])

    u_err = ioda_vars.Variable(
        obs_space,
        'eastward_wind',
        np.float32,
        ('Location',),
        fillval=np.nan,
        group='ObsError'
    )
    u_err.write_data(np.full(nlocs, 1.5))  # example

    v_err = ioda_vars.Variable(
        obs_space,
        'northward_wind',
        np.float32,
        ('Location',),
        fillval=np.nan,
        group='ObsError'
    )
    v_err.write_data(np.full(nlocs, 1.5))

    u_qc = ioda_vars.Variable(
        obs_space,
        'eastward_wind',
        np.int32,
        ('Location',),
        fillval=0,
        group='QCFlags'
    )
    u_qc.write_data(np.zeros(nlocs, dtype=np.int32))

    v_qc = ioda_vars.Variable(
        obs_space,
        'northward_wind',
        np.int32,
        ('Location',),
        fillval=0,
        group='QCFlags'
    )
    v_qc.write_data(np.zeros(nlocs, dtype=np.int32))




    

    obs_space.write_attr(
        'ioda_version',
        np.int32(2),
        ioda_attr.AttrType.Int
    )


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input', required=True)
    parser.add_argument('-o', '--output', required=True)
    args = parser.parse_args()

    data = read_input(args.input)
    write_ioda(data, args.output)

