<!DOCTYPE html>
<html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
    <head profile="<?php print $grddl_profile ?>">
        <?php print $head; ?>
        <title><?php print $head_title; ?></title>
        <?php print $meta; ?>
        <?php print $styles; ?>
    </head>
    <body class="<?php print $classes; ?>"<?php print $attributes; ?>>
        <?php print $page; ?>
        <?php print $scripts; ?>
<?php print $belatedpng; ?>
    </body>
</html>
