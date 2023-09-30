# ESM: Eknous' Screeching Monstrosity

## specs:
### 4 PSG channels
- - 12-bit pitch resolution, 4-bit volume resolution
  - thin pulse or 50% duty pulse
  - pulse or noise
    - long noise on channels 1 and 2, short noise on 3 and 4
- global PSG volume control (8-bit)
- 8-bit PRNG output
### Sample counter
- 16-bit address counter for external memory device (e.g. RAM)
  - 15-bit counter speed (pitch) resolution
  - loop start and end (16-bit)

## building and running the testing tool
```
make prep
make build
make run
```
