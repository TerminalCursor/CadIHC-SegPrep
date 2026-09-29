** Example Usage

For the following labelling input structure
```
data/
├── test
│   ├── HT-A1
│   │   ├── Label1.tif
│   │   ├── OtherLabel.tif
│   │   ├── NameMe.tif
│   │   └── A1.labels.tif[4].tif
│   └── HT-A2
│       ├── Label.tif
│       ├── 2.tif
│       └── A2.tif
└── train
    ├── HT-B1
    │   └── B1.labels.tif[0].tif
    ├── HT-B2
    │   ├── B2.tif\ [1].tif
    │   ├── B2.tif\ [2].tif
    │   └── B2.tif\ [3].tif
    ├── HT-C5
    │   ├── C5.tif\ [1].tif
    │   ├── C5.tif\ [2].tif
    │   └── C5.tif\ [3].tif
    └── HT-J9
        └── J9.tif[1].tif
```

Simply run:
```sh
./buildall.sh
```

To get:
```
output/
├── test
│   ├── HT-A1.bin
│   ├── HT-A1.png
│   ├── HT-A1_seg.npy
│   ├── HT-A1.tif
│   ├── HT-A2.bin
│   ├── HT-A2.png
│   ├── HT-A2_seg.npy
│   └── HT-A2.tif
└── train
    ├── HT-B1.bin
    ├── HT-B1.png
    ├── HT-B1_seg.npy
    ├── HT-B1.tif
    ├── HT-B2.bin
    ├── HT-B2.png
    ├── HT-B2_seg.npy
    ├── HT-B2.tif
    ├── HT-C5.bin
    ├── HT-C5.png
    ├── HT-C5_seg.npy
    ├── HT-C5.tif
    ├── HT-J9.bin
    ├── HT-J9.png
    ├── HT-J9_seg.npy
    └── HT-J9.tif
```
