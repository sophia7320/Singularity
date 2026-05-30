vim.opt.number = true
vim.opt.relativenumber = true

vim.opt.cursorline = true
vim.opt.termguicolors = true
vim.opt.cursorlineopt = "number"
vim.opt.signcolumn = "yes"
vim.opt.scrolloff = 4
vim.opt.sidescrolloff = 8

vim.opt.ignorecase = true
vim.opt.smartcase = true
vim.opt.hlsearch = false
vim.opt.wrapscan = false

vim.opt.expandtab = true
vim.opt.tabstop = 4
vim.opt.shiftwidth = 4
vim.opt.softtabstop = 4
vim.opt.autoread = true
vim.opt.smartindent = true
vim.opt.wrap = false

vim.opt.clipboard = "unnamedplus"

vim.opt.splitbelow = true
vim.opt.splitright = true

vim.g.mapleader = " "
vim.g.loaded_netrw = 1
vim.g.loaded_netrwPlugin = 1

vim.opt.wildignore = { "*.o", "*.a", "__pycache__", ".git" }

vim.swapfile = false
vim.undofile = true
vim.undodir = vim.fn.stdpath("data") .. "/undo"

vim.updatetime = 200
vim.pumblend = 10
vim.pumheight = 10
vim.timeoutlen = 300
vim.smoothscroll = true
vim.splitkeep = "screen"

vim.foldlevel = 99
vim.foldenable = false

if vim.env.SSH_CONNECTION then
    vim.opt.clipboard = ""
end

vim.opt.ttimeoutlen = 0
