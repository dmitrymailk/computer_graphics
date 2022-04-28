# Web Computer Graphics

### [link to website](https://dmitrymailk.github.io/computer_graphics/)

## Useful links

- [host website on github pages](https://learnvue.co/2020/09/how-to-deploy-your-vue-app-to-github-pages/#how-does-github-pages-work)

## Как загрузить сайт на github pages

1. Закоммитить свой прогресс
2. Спуститься в root дирректорию
3. git add путь**к**папке**с**сайтом -f (это если она была добавленва в gitignore)
4. git subtree push --prefix путь**к**папке**с**сайтом origin gh-pages (git subtree push --prefix web-computer-graphics/dist/ origin gh-pages)
5. Перейти в настройки и открыть раздел _pages_, там будет ссылка на сайт
