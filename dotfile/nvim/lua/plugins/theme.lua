return {
    -- 第一部分：下载并配置 catppuccin 主题
    {
        "catppuccin/nvim",
        name = "catppuccin",
        opts = {
            flavour = "mocha",
            transparent_background = true,
            float = {
                transparent = true, -- enable transparent floating windows
            },
        },
        config = function(_, opts)
            require("catppuccin").setup(opts)
            vim.cmd.colorscheme("catppuccin")
        end,
    },

    -- 第二部分：覆写 LazyVim 配置，安全地将默认主题指定为 catppuccin
    {
        "LazyVim/LazyVim",
        opts = {
            colorscheme = "catppuccin",
        },
    },
}
