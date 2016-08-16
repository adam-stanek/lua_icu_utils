icu_utils
=========

Library providing interface to selected utilities from ICU project.

Check the specs for examples

## Functions

```lua
-- normalizes UTF-8 string from NKD to NFC
icu_utils.normalize(str)

-- converts string to lower case
icu_utils.lower(str)

-- converts string to upper case
icu_utils.upper(str)

-- converts string to title case
icu_utils.capitalize(str)

-- performs transliteration of a string into latin
-- replaces any accented characters with their unaccented equivalent
icu_utils.latinize(str)
```

## Usage

```bash
sudo apt-get install libicu-dev
luarocks make
```
