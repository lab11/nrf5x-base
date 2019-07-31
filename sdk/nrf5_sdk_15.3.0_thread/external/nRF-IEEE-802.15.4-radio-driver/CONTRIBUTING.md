# Contribution

Feel free to file code related issues on [GitHub Issues](https://github.com/NordicSemiconductor/nRF-IEEE-802.15.4-radio-driver/issues) and/or submit a pull request. In order to accept your pull request, we need you to sign our Contributor License Agreement (CLA). You will see instructions for doing this after having submitted your first pull request. You only need to sign the CLA once, so if you have already done it for another project in the NordicSemiconductor organization, you are good to go.

## Coding Standard

The nRF IEEE 802.15.4 radio driver uses Nordic Semiconductor Firmware Coding Standard on all code located in [src](src) directory.

Before submitting a pull request, please ensure that your code passes the baseline code style checks.

Use the `pretty.sh` script to automatically check or reformat the code:

```bash 
# Perform style checking
./scripts/pretty.sh check

# Perform full reformat by modyfing all source code
./scripts/pretty.sh
```

The `pretty.sh` script relies on [uncrustify-0.66.1](https://github.com/uncrustify/uncrustify/releases/tag/uncrustify-0.66.1).
