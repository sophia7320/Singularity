return {

    {
        "mason-org/mason.nvim",
        lazy = false,
        opts = {
            ui = {
                border = "rounded",
                icons = {
                    package_installed = "✓", -- 已安装图标
                    package_pending = "➜", -- 安装中图标
                    package_uninstalled = "✗",
                },
            },
        },
    },
    {
        "neovim/nvim-lspconfig",
        dependencies = { "mason-org/mason.nvim" },
    },

    {
        "WhoIsSethDaniel/mason-tool-installer.nvim",

        opts = {
            ensure_installed = {
                "stylua",
                "ruff",
                "prettierd",
                --"goimports",
                "luacheck",
                "clang-format",
                "codelldb",
            },
        },
    },
    {
        "mason-org/mason-lspconfig.nvim",
        dependencies = {
            "mason-org/mason.nvim",
            "neovim/nvim-lspconfig",
        },
        opts = {
            ensure_installed = {
                "lua_ls",
                "ts_ls",
                -- "rust_analyzer",
                "pyright",
                "clangd",
            },
            automatic_enable = true,
        },
    },

    {
        "mfussenegger/nvim-lint",
        event = { "BufReadPre", "BufNewFile" },

        opts = {
            linters_by_ft = {
                python = { "ruff" },
                javascript = { "eslint_d" },
                typescript = { "eslint_d" },
                lua = { "luacheck" },
            },
        },
    },
    -- 文件路径: ~/.config/nvim/lua/plugins/dap.lua (在上述配置后追加)

    -- 4. Mason 调试适配器管理
    {
        "jay-babu/mason-nvim-dap.nvim",
        dependencies = {
            "mason-org/mason.nvim", -- 依赖 Mason 核心
            "mfussenegger/nvim-dap", -- 依赖 nvim-dap
        },
        opts = {
            -- 确保这些适配器在 Mason 中自动安装
            ensure_installed = {
                "debugpy", -- Python
                "codelldb", -- C, C++, Rust
                "delve", -- Go
                -- "js-debug-adapter", -- JavaScript/TypeScript
            },
            automatic_installation = true, -- 自动安装未安装的适配器
        },
        config = function(_, opts)
            require("mason").setup()
            require("mason-nvim-dap").setup(opts)
            -- 可以根据需要，为不同语言配置调试器

            -- lua/config/dap/python.lua

            local dap = require("dap") -- (1) 加载 nvim-dap

            -- ─── 适配器：告诉 nvim-dap 如何启动 debugpy ──────────
            dap.adapters.python = {
                type = "executable", -- (2) 启动子进程，通过 stdio 通信
                command = "python3", -- (3) Python 解释器路径
                args = { "-m", "debugpy.adapter" }, -- (4) 以模块方式运行 debugpy 适配器
                -- 等价于 shell: python3 -m debugpy.adapter
            }

            -- ─── 配置：告诉 debugpy 如何运行你的代码 ──────────────
            dap.configurations.python = {
                {
                    type = "python", -- (5) 必须匹配适配器键名
                    request = "launch", -- (6) 启动新进程
                    name = "Launch file", -- (7) 选择菜单显示名
                    program = "${file}", -- (8) ${file} = 当前文件路径
                    console = "integratedTerminal", -- (9) 输出到 Neovim 终端 [默认值，可选]
                    justMyCode = false, -- (10) false: 可进入第三方库代码

                    -- (11) pythonPath — 动态选择 Python 解释器
                    pythonPath = function()
                        local cwd = vim.fn.getcwd()
                        if vim.fn.executable(cwd .. "/venv/bin/python") == 1 then
                            return cwd .. "/venv/bin/python" -- .venv/
                        elseif vim.fn.executable(cwd .. "/.venv/bin/python") == 1 then
                            return cwd .. "/.venv/bin/python" -- ./venv/
                        end
                        return "python3" -- 系统默认
                    end,
                },
            }
        end,
    },
}
