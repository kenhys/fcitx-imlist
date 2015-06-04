# fcitx-imlist

`fcitx-imlist` is a small program to show or change input method list.

There is already similar program exists - `fcitx-remote` command.
It can support to switch input method by `fcitx-remote -s INPUT_METHOD`,
but It doesn't support to change the order of input method list.
That is why I wrote this experimental program.

# Usage

## How to show input methods?

Use `-l` option to list input methods which is enabled.

    % fcitx-imlist -l
    fcitx-keyboard-jp (キーボード - 日本語) [enabled]
    mozc (Mozc) [enabled]
    fcitx-keyboard-us (キーボード - 英語 (US)) [enabled]

## How to show all input methods?

Use `-l` and `-v` option at the same time to list all input methods.

    % fcitx-imlist -l -v
    fcitx-keyboard-jp (キーボード - 日本語) [enabled]
    mozc (Mozc) [enabled]
    fcitx-keyboard-us (キーボード - 英語 (US)) [enabled]
    fcitx-keyboard-af (キーボード - アフガニスタン語) [disabled]
    fcitx-keyboard-af-fa-olpc (キーボード - アフガニスタン語 - ペルシア語 (アフガニスタン、ダリー語 OLPC)) [disabled]
    fcitx-keyboard-af-olpc-ps (キーボード - アフガニスタン語 - パシュト語 (アフガニスタン、OLPC)) [disabled]
    ...

## How to change the order of input methods?

Use `-s` option with input methods list to change the order.

    % fcitx-imlist -s fcitx-keyboard-us,mozc
    fcitx-keyboard-us (キーボード - 英語 (US)) [enabled]
    mozc (Mozc) [enabled]
    fcitx-keyboard-jp (キーボード - 日本語) [enabled]

