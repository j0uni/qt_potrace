# Qt Potrace

A Qt-based GUI application for converting bitmap images to vector graphics using the Potrace algorithm.

## Features

- Load and display bitmap images (PNG, JPG, BMP)
- Convert images to grayscale
- Automatic downscaling of large images (>1024px)
- Convert images to vector graphics using Potrace
- Preview the vectorized result
- Export to SVG format
- Remember last used file

## Building

### Prerequisites

- Qt 6.0 or later

### Build Instructions

This project works directly in Qt Creator. No need to install anything extra.

## License

This project is licensed under the GNU General Public License v2 (GPL-2.0).

### Potrace Library

This project includes a copy of the Potrace library, which is also licensed under the GNU General Public License v2 (GPL-2.0). The Potrace library is copyright (C) 2001-2019 Peter Selinger.

The Potrace library is included in the `potrace` directory of this project. You can find the original Potrace library at [http://potrace.sourceforge.net/](http://potrace.sourceforge.net/).

### License Compliance

Since both this project and the included Potrace library are licensed under GPL-2.0, you are free to:

1. Use this software for any purpose
2. Study how it works and modify it
3. Distribute it
4. Improve it and release your improvements

When distributing this software, you must:

1. Include the complete source code
2. Include the GPL-2.0 license text
3. Include the copyright notices
4. State significant changes made to the files
5. Include the offer to provide the source code on request

For more details about the GPL-2.0 license, see the [COPYING](potrace/COPYING) file in the Potrace library directory.
